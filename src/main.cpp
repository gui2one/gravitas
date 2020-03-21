#include "pch.h"
#include "core.h"
#include "3d_utils.h"
#include "orbital_body.h"
#include "trajectory_display.h"
#include "iss_data.h"

#include "bitmap_font.h"
#include "bitmap_font_atlas.h"

#include <thread>
#include <chrono>

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library  ft_library;
FT_Face     ft_face;



char * test_char;

using Orbiter::ISSData;
using Orbiter::IssPositionData;
using Orbiter::Vertex;
using Orbiter::Polyline;
using Orbiter::PolylineObject;
using Orbiter::TrajectoryDisplay;
using Orbiter::OrbitalBody;
using Orbiter::SceneBackground;


Orbiter::BitmapFont bitmap_font;
Orbiter::BitmapFontAtlas font_atlas;
Shader font_shader;

GtkApplication * app;

GtkWidget * UI_window;
GtkWidget * GL_window;
GtkWidget * frame;
GtkWidget * button;

GtkWidget * btn_add_planet;

GtkWidget * tree_view;
GtkWidget * label;
GtkWidget * fps_label;
GtkTreeSelection * selection; 

bool b_auto_rotate = true;
GtkWidget * auto_rotate_checkbtn;
GtkWidget * fps_checkbtn;
float fps_refresh_delay = 1.0;
unsigned int fps_refresh_millis_counter = 0;
unsigned int fps_refresh_frames_counter = 0;
char fps_txt[255];

Timer timer;

GtkWidget * grid;
GtkWidget * gl_area;

double time_scale = 1.0;
GtkWidget * time_scale_spin;

double system_scale = 1.0 / (6378.137 * 2.0); // max radius of earth at sea level (at the equator)
TrajectoryDisplay traj_test, unit_traj;
bool traj_test_update_ready = false;
unsigned int iss_max_positions = 2000;

ISSData * iss_data = ISSData::getInstance();

static void add_iss_position()
{
	while(true)
	{
		IssPositionData d = iss_data->send_request();
		iss_data->m_data.push_back(d);
		
		if( iss_data->m_data.size() > iss_max_positions)
		{
				iss_data->m_data.erase(iss_data->m_data.begin(), iss_data->m_data.begin() + iss_data->m_data.size() - iss_max_positions);
		}
		
		char str[256];
		//~ sprintf(&str[0], " %d positions collected", (int)iss_data->m_data.size());
		sprintf(&str[0], "lat : %.3f , lon : %.3f ", iss_data->m_data[iss_data->m_data.size()-1].latitude, iss_data->m_data[iss_data->m_data.size()-1].longitude);
		gtk_label_set_text(GTK_LABEL(label), str);
		

		
		traj_test_update_ready = true;
	
		
		std::this_thread::sleep_for((std::chrono::seconds)5);
	}
}

std::thread thread(add_iss_position);


bool camera_orbit_started = false;
bool b_show_fps = true;
float camera_u = 0.5f;
float camera_v = 0.0f;
float camera_orbit_radius = 3.0f;
float old_mouse_x = 0.0f;
float old_mouse_y = 0.0f;
float mouse_pause_time = 0.0;

static void refresh_fps()
{
	
	
	if( fps_refresh_millis_counter > fps_refresh_delay * 1000)
	{
		fps_refresh_millis_counter = 0;
		sprintf(fps_txt, " %.1f fps", (1.0f/ (timer.getDeltaMillis() / 1000.0f)));		

		if (b_show_fps)
		{
			//bitmap_font.setText(fps_txt);
			bitmap_font.setText("abc^");

			bitmap_font.renderTextureFromAtlas();
		}
	}
	fps_refresh_millis_counter += timer.getDeltaMillis();
	fps_refresh_frames_counter++;
}

static void update_traj_test()
{
	if(traj_test_update_ready)
	{
		traj_test_update_ready = false;
		
		Orbiter::Polyline poly;
		std::vector<Vertex> vertices;
		
		int num_elements = iss_data->m_data.size();
		int counter = 0;
		vertices.reserve(num_elements);
		for(auto& data : iss_data->m_data)
		{
				Vertex vtx;
				float u = ((data.latitude + 90.0f ) / 180.0) ;
				float v = ((data.longitude + 180.0f ) / 360.0) ;
				glm::vec3 euclidian = Orbiter::Utils::polar_to_euclidian( (1.0-u)* PI, v* PI * 2, 0.5f + (data.altitude * system_scale));
				vtx.position = glm::vec3(euclidian);
				vtx.color = glm::vec4(1.0f, 1.0f, 1.0f, (float)counter/(num_elements-1));
				vertices.emplace_back(vtx);
				
				counter++;
		}
		
		poly.vertices = vertices;	
		
		traj_test.polyline_object.setPolyline(poly);	
		
	}
}




std::vector<std::shared_ptr<OrbitalBody > > planets;
std::shared_ptr<OrbitalBody> earth, moon;
Orbiter::Camera camera;

SceneBackground scene_bg;







float planet_rotation_speed = 0.0f;

static void iss_sim_step()
{
	auto& positions = iss_data->getData();
	
	size_t num = 2;
	if( positions.size() > num)
	{
		IssPositionData gps1 = positions[ positions.size()-2];
		int time_stamp_1 = gps1.timestamp;
		float u = ((gps1.latitude + 90.0f ) / 180.0) ;
		float v = ((gps1.longitude + 180.0f ) / 360.0) ;		
		glm::vec3 pos1 = Orbiter::Utils::polar_to_euclidian((1-u )* PI, v * PI * 2.0f, 0.5f + (gps1.altitude * system_scale)); 


		IssPositionData gps2 = positions[ positions.size()-1];
		int time_stamp_2 = gps2.timestamp;
		u = ((gps2.latitude + 90.0f ) / 180.0) ;
		v = ((gps2.longitude + 180.0f ) / 360.0) ;		
		glm::vec3 pos2 = Orbiter::Utils::polar_to_euclidian((1-u) * PI, v * PI * 2.0f, 0.5f + (gps2.altitude * system_scale));	
		
		
		//~ float sec_elapsed = (float)(time_stamp_2 - time_stamp_1);
		
		//~ float dist = glm::distance(pos1, pos2) / system_scale;
		
		


        double radius = (6378.137) + (double)gps2.altitude; // max radius of earth at sea level (at the equator)
        
        double dot = glm::dot(glm::normalize(pos1), glm::normalize(pos2));
        double angle = acos(dot);
        
        double dist_V2 = angle * radius;

		
		double daynum_diff = gps2.daynum - gps1.daynum;

		
		
		double sec_elapsed_V2 = daynum_diff * 23.93 * 60.0 * 60.0;
		
		
		
		double vel = (dist_V2  / sec_elapsed_V2) *( 60.0 * 60.0);
		
        //~ printf("angle(radians) :  %.3lf\n", angle);        
        //~ printf("distance :  %.3lf\n", dist_V2);   		
		//~ printf("altitude 1 : %.2lf\n", gps1.altitude);
		//~ printf("altitude 2 : %.2lf\n", gps2.altitude);
		
		//~ printf("\ndaynum 1 : %lf\n", gps1.daynum);
		//~ printf("daynum 2 : %lf\n", gps2.daynum);		
		
		//~ printf("Difference : %lf\n", daynum_diff);
		//~ printf("Difference (secs): %.4lf\n", daynum_diff * 23.93 * 60.0 * 60.0);		
		
		//~ printf("distance : %.8f \n", dist);
		//~ printf("velocity : %.2lfkm/h \n", vel );
		//~ printf("time_step : %.4f \n", sec_elapsed);
		//~ printf("--------------------------------\n");
	}
}

enum
{
	COLUMN_ID = 0,
	COLUMN_NAME = 1,
	N_COLUMNS
};

void init_list(GtkWidget *list) 
{
	GtkListStore *store;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;



	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes("ID", renderer,
									"text", COLUMN_ID,
									NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	column = gtk_tree_view_column_new_with_attributes("NAME", renderer,
									"text", COLUMN_NAME,
									NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_INT, G_TYPE_STRING);

	gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
	  GTK_TREE_MODEL(store));

	g_object_unref(store);
}

void add_to_list(GtkWidget *list, int id, const gchar * str)
{
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 
	COLUMN_ID, id,
	COLUMN_NAME, str,
	 -1);
	 
  gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
      GTK_TREE_MODEL(store));	 

	//~ g_object_unref(store);
}

void add_planet()
{
	//~ init_list(tree_view);
	add_to_list(tree_view, 4, "other data");
	
	//~ gtk_widget_queue_draw(tree_view);
}

void on_changed(GtkWidget *widget, gpointer _label) 
{ 
  //~ g_print("changed\n");
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) 
  {
    gtk_tree_model_get(model, &iter, COLUMN_NAME, &value,  -1);
    gtk_label_set_text(GTK_LABEL(_label), value);
    g_free(value);
  }
}

static void print_hello(GtkSpinButton *spin, gpointer data)
{
	//~ g_print("hello From GTK 3.0\n");	
	time_scale = gtk_spin_button_get_value(spin);
	
}

static gboolean on_realize(GtkGLArea * gl_area, GdkGLContext * context)
{

	
	gtk_gl_area_make_current(gl_area);
	
	if( gtk_gl_area_get_error(GTK_GL_AREA(gl_area)) != NULL)
	{
			g_print("error with gl area\n");
			return false;
	}
	
	

	if( glewInit() != GLEW_OK)
	{
		g_print("error with GLEW\n");
		
	}
	
	const unsigned char * gl_version = glGetString(GL_VERSION);
	const unsigned char * glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	g_print("\nOpengl Version Info \n");
	g_print("---------------------------\n");
	g_print("\t-OpenGL : %s\n", gl_version);
	g_print("\t-Shading Language : %s\n", glsl_version);
	g_print("---------------------------\n");
	

	

	

	//int inc = 0;

	//for (auto& info : font_atlas.getGlyphsInfos())
	//{

	//	printf(" %03d char : '%c'\n", inc, info.ch);
	//	inc++;
	//}
	
	int allocated_width = gtk_widget_get_allocated_width(GTK_WIDGET(gl_area));
	int allocated_height = gtk_widget_get_allocated_height(GTK_WIDGET(gl_area));	

	float ratio = (float)allocated_width / allocated_height;	
	
	

	font_shader.loadVertexShaderSource("../../src/res/shaders/font_shader.vert");
	font_shader.loadFragmentShaderSource("../../src/res/shaders/font_shader.frag");
	font_shader.createShader();	
	
	bitmap_font.init(font_shader);
	bitmap_font.setAtlas(font_atlas);
	bitmap_font.setText("AB");
	bitmap_font.setHAlign(Orbiter::FONT_HALIGN_START);
	bitmap_font.setVAlign(Orbiter::FONT_VALIGN_BOTTOM);
	
	//~ bitmap_font.setText((const char *)test_char);
	bitmap_font.renderTextureFromAtlas();
	
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	//~ glCullFace(GL_BACK);	
	

	
	camera.setProjection(glm::mat4(1.0f) * glm::perspective(45.0f, ratio, 0.01f, 100.0f));
	
	
	camera.orbit(camera_u, camera_v, camera_orbit_radius);
	// init planets
	earth = std::make_shared<OrbitalBody>();
	
	earth->setRadius(12742.0f / 2.0f * system_scale);
	earth->init();	
	earth->loadTexture("images/2k_earth_daymap.jpg");
	earth->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	planets.push_back(earth);	
	
	moon = std::make_shared<OrbitalBody>();
	
	float dist_earth_moon = 363104.0f * system_scale; 
	
	moon->setRadius(3474.0f / 2.0f * system_scale);
	moon->setOrbitalParent(earth);
	moon->init();	
	moon->loadTexture("images/2k_moon.jpg");
	moon->setPositionX(dist_earth_moon);
	planets.push_back(moon);	
		
	scene_bg.init();
	traj_test.init();
	unit_traj.init();
	
	
	Orbiter::Polyline poly;
	std::vector<Vertex> vertices;
	Vertex vtx;
	vtx.position = glm::vec3(-0.5f, 1.0f, 0.0f);
	poly.vertices.push_back(vtx);
	vtx.position = glm::vec3(0.5f, 1.0f, 0.0f);
	poly.vertices.push_back(vtx);	
	
	unit_traj.polyline_object.setPolyline(poly);
	
	
	timer.start();
	
	return true;
	
}

static void on_gl_resize(GtkGLArea * gl_area, gint width, gint height, gpointer user_data)
{

	float ratio = (float)width / height;	
	camera.setProjection(glm::mat4(1.0f) * glm::perspective(45.0f, ratio, 0.01f, 100.0f));	

}

static gboolean render(GtkGLArea * gl_area, GdkGLContext *context)
{

	timer.update();
	refresh_fps();

	int allocated_width = gtk_widget_get_allocated_width(GTK_WIDGET(gl_area));
	int allocated_height = gtk_widget_get_allocated_height(GTK_WIDGET(gl_area));
	//~ g_print("\nrectangle width: %d\n", allocated_width);	
	
	float ratio = (float)allocated_width / (float)allocated_height;
		
	
	update_traj_test();
	
	iss_sim_step();

	

	
	gtk_label_set_text(GTK_LABEL(fps_label), fps_txt);
	if( b_show_fps)
	{
		bitmap_font.setPosition(glm::vec2(-(float)allocated_width / 2.0, -(float)allocated_height / 2.0));
		

		//~ bitmap_font.setText((const char *) fps_txt);
		//~ bitmap_font.renderTextureFromAtlas();
	}
	GLCall(glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ));
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 1.0, 0.0, 0.0);
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));	


	
	glOrtho(-1.0 * ratio, 1.0 * ratio, -1.0, 1.0 , -1.0, 1.0);

	

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	GLCall(glFrontFace(GL_CW));
	GLCall(glCullFace(GL_BACK));	
	
		
		scene_bg.render(camera);
	
		
	
	GLCall(glFrontFace(GL_CCW));
	glDisable(GL_CULL_FACE);
	
		
	
	glEnable(GL_DEPTH_TEST);
	
	
	float second_fraction = (float)timer.getDeltaMillis() / 1000.0;
	float angle = (360.0) * (second_fraction /( 60 * 60 * 24.0));


	earth->setRotationZ( earth->getRotation().z + (angle * time_scale));
	earth->render(camera);			
	
	moon->render(camera);
	
	traj_test.setRotationZ( traj_test.getRotation().z + (angle * time_scale));
	traj_test.render(camera);
	
	//~ unit_traj.render(camera);
	
	
	
	// render fonts
	if( b_show_fps)
	{
		GLCall(glDisable(GL_DEPTH_TEST));
			bitmap_font.draw(allocated_width , allocated_height);	
		GLCall(glEnable(GL_DEPTH_TEST));
	}
	
	mouse_pause_time += second_fraction;

	if(mouse_pause_time > 10.0 && b_auto_rotate)
	{
		camera_v +=  0.05 * second_fraction;
		camera.orbit(camera_u, camera_v, camera_orbit_radius);

	}
	
	
	gtk_gl_area_queue_render(gl_area);
	
	
	return true;
}

static gboolean gl_area_click( GtkWidget * widget, GdkEventButton * event)
{
	if( event->button == 1)
	{
		camera_orbit_started = true;
		old_mouse_x = event->x;
		old_mouse_y = event->y;
	
		//~ g_print("clicked button %d \n", event->button);
		//~ g_print("- X : %.3f , Y : %.3f \n", (double)event->x, (double)event->y);
		//~ g_print("--------------------------------\n");
	}
	
	mouse_pause_time = 0.0;
	//~ g_print("pause time : %.3f \n", mouse_pause_time);
	return TRUE;
}

static gboolean gl_area_release( GtkWidget * widget, GdkEventButton * event)
{	
	if( event->button == 1)
		camera_orbit_started = false;
		
	//~ g_print("released button %d \n", event->button);
	//~ g_print("- X : %.3f , Y : %.3f \n", (double)event->x, (double)event->y);
	//~ g_print("--------------------------------\n");
	return TRUE;
}

static gboolean gl_area_motion( GtkWidget * widget, GdkEventMotion * event)
{
	if( camera_orbit_started)
	{

		
		float delta_x = (float)event->x - old_mouse_x;
		float delta_y = event->y - old_mouse_y;
		
		old_mouse_x = event->x;
		old_mouse_y = event->y;

		
		camera_v += -delta_x * 0.02; // * (1.0/delta);
		camera_u += -delta_y * 0.02; // * (1.0/delta);
		
		
		// clamp vertical polar coord position		
		if( camera_u > PI - (PI*0.05))
		{
			camera_u = PI - (PI*0.05);
		}else if( camera_u < PI*0.05)
		{
			camera_u = PI*0.05;
		}
		
		// finally orbit camera
		camera.orbit(camera_u, camera_v, camera_orbit_radius);
		
	}else{
		
	}
	return TRUE;
}

static gboolean gl_area_scroll( GtkWidget * widget, GdkEventScroll * event)
{
	//~ g_print("%d\n", event->direction);

	
	
	float mult = 1.05;
	
	if( event->direction == GDK_SCROLL_UP )
	{
		mult = 0.95;
	}
	
	camera_orbit_radius *= mult;
	
	camera.orbit(camera_u, camera_v, camera_orbit_radius);
	return TRUE;
}

static void on_auto_rotate_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
	b_auto_rotate = gtk_toggle_button_get_active(togglebutton);
}

static void on_show_fps_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
	b_show_fps = gtk_toggle_button_get_active(togglebutton);
}


static gboolean app_quit(GtkWidget * widget, GdkEvent * event, gpointer user_data)
{
	g_print("quitting Orbiter\n");
	//~ gtk_window_close(GTK_WINDOW(UI_window));
	g_application_quit(G_APPLICATION(app));
	return TRUE;
}

static void activate( GtkApplication *app, gpointer user_data)
{

	
	
	GL_window = gtk_application_window_new(app);
	gtk_window_set_default_size(GTK_WINDOW(GL_window), 1280,720);
	gtk_window_set_title(GTK_WINDOW(GL_window), "Orbiter GL");
	//gtk_window_set_type_hint(GTK_WINDOW(GL_window), GDK_WINDOW_TYPE_HINT_DIALOG);
	
	
	
	UI_window = gtk_application_window_new(app);
	gtk_window_set_transient_for(GTK_WINDOW(GL_window),GTK_WINDOW(UI_window));
	gtk_window_set_title(GTK_WINDOW(UI_window), "Orbiter UI");
	g_signal_connect(GTK_WINDOW(UI_window), "delete_event", G_CALLBACK(app_quit), NULL);	
	gtk_window_set_default_size(GTK_WINDOW(UI_window), 500,300);	
	gtk_container_set_border_width(GTK_CONTAINER(UI_window), 10);
	

	grid = gtk_grid_new();

	gtk_widget_set_vexpand(GTK_WIDGET(grid), TRUE);
	gtk_widget_set_hexpand(GTK_WIDGET(grid), TRUE);
	
	
	GtkAdjustment * adj = gtk_adjustment_new(1.0, 1.0, 1000000000.0, 1.0, 5.0, 0.0);
    time_scale_spin = gtk_spin_button_new(adj, 10.0, 3);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(time_scale_spin), time_scale);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(time_scale_spin), TRUE);
	g_signal_connect(GTK_SPIN_BUTTON(time_scale_spin), "value-changed", G_CALLBACK(print_hello), NULL);	
	gtk_grid_attach(GTK_GRID(grid), time_scale_spin, 0,0,2,1);
	
	

	
	btn_add_planet = gtk_button_new_with_label("add planet");
	g_signal_connect(btn_add_planet, "clicked", G_CALLBACK(add_planet), NULL);	
	gtk_grid_attach(GTK_GRID(grid), btn_add_planet, 2,0,1,1);
									
	auto_rotate_checkbtn = gtk_check_button_new_with_label("Camera auto rotate");
	g_signal_connect(GTK_TOGGLE_BUTTON(auto_rotate_checkbtn), "toggled", G_CALLBACK(on_auto_rotate_toggled), NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(auto_rotate_checkbtn), b_auto_rotate);
	gtk_grid_attach(GTK_GRID(grid), auto_rotate_checkbtn, 3,0,1,1);

	
	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 4,0,1,1);
	tree_view = gtk_tree_view_new();
	
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), TRUE);
	init_list(tree_view);
	add_to_list(tree_view, 1, "hello ??");
	add_to_list(tree_view, 2, "item 2");
	add_to_list(tree_view, 3, "other data");
	
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));

	g_signal_connect(selection, "changed", G_CALLBACK(on_changed), label);	
	
	gtk_grid_attach(GTK_GRID(grid), tree_view, 0,1,2,1);
	
	gl_area = gtk_gl_area_new();
	
	gtk_gl_area_set_required_version(GTK_GL_AREA(gl_area), 5, 0);

	gtk_widget_add_events(gl_area, 
							GDK_BUTTON_PRESS_MASK | 
							GDK_BUTTON_RELEASE_MASK | 
							GDK_POINTER_MOTION_MASK | 
							GDK_SCROLL_MASK | 
							GDK_BUTTON_MOTION_MASK);
	gtk_widget_set_hexpand(gl_area, true);
	gtk_widget_set_vexpand(gl_area, true);
	
	g_signal_connect(gl_area, "button_press_event", G_CALLBACK(gl_area_click), NULL);
	g_signal_connect(gl_area, "button_release_event", G_CALLBACK(gl_area_release), NULL);
	g_signal_connect(gl_area, "motion_notify_event", G_CALLBACK(gl_area_motion), NULL);
	g_signal_connect(gl_area, "scroll_event", G_CALLBACK(gl_area_scroll), NULL);
	g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), NULL);
	g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);
	g_signal_connect(gl_area, "resize", G_CALLBACK(on_gl_resize), NULL);
	
	gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(gl_area), TRUE);
	
	
	gtk_window_set_attached_to(GTK_WINDOW(GL_window), UI_window);
	
	gtk_container_add(GTK_CONTAINER(UI_window), grid);	
	gtk_container_add(GTK_CONTAINER(GL_window), gl_area);	
	
	
	GtkWidget * fps_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	
	fps_checkbtn = gtk_check_button_new_with_label("Show fps");
	g_signal_connect(GTK_TOGGLE_BUTTON(fps_checkbtn), "toggled", G_CALLBACK(on_show_fps_toggled), NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fps_checkbtn), b_show_fps);
	gtk_container_add(GTK_CONTAINER(fps_box), fps_checkbtn);
	
	fps_label = gtk_label_new("fps");
	gtk_label_set_xalign(GTK_LABEL(fps_label), 0.0f);
	gtk_container_add(GTK_CONTAINER(fps_box), fps_label);
	
	gtk_grid_attach(GTK_GRID(grid), fps_box, 0,2,2,1);
	//~ gtk_container_add(GTK_CONTAINER(UI_window), fps_box);	
	
	gtk_widget_show_all(GL_window);
	gtk_widget_show_all(UI_window);
	

	
}

int main(int argc, char ** argv)
{	
	//~ gtk_init(&argc, &argv);
	
	int status;

	
	//~ return 0;

	app = gtk_application_new("gui2one.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	//~ g_signal_connect(app, "parsing-error", G_CALLBACK(on_css_parsing_error), NULL);
	status = g_application_run( G_APPLICATION (app), argc, argv);
	
	g_object_unref(app);
	
	
	//~ g_print("Good Bye GTK\n");
	return status;
}
