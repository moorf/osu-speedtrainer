#include <iostream>
#include <thread>
#include <chrono>
#include <allegro5\allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <allegro5/allegro_windows.h>
#include <cstdio>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int clicks = 0;
int alternate = 0;
int alternate2 = 0;
int fail = 1;
bool start = false;
int time_window = 1000;
int start_time = 0;
int error = 0;
int b1 = ALLEGRO_KEY_Z;
int b2 = ALLEGRO_KEY_X;
float bpm = 150;
int late = 0;
int restart = true;
int milliseconds = 0;
int punishmentms = 30;
int hm = 0;
std::string config_filename = "config.cfg";
bool disablealt = 0;
int consolemovex;
int consolemovey;
int consolesizex = 500;
int consolesizey = 400;

int inputmovex;
int inputmovey = 400;
int inputsizex = 200;
int inputsizey = 200;

int quitbutton = ALLEGRO_KEY_Q;
char restartbutton = ALLEGRO_KEY_TILDE;
char bpmbutton = ALLEGRO_KEY_B;


bool exitt = false;


void create_default_config(const std::string& filename) {
	std::ofstream f(filename);
	if (f.is_open()) {
		f	<< "z\n"
			<< "x\n"
			<< "180\n"
			<< "30\n"
			<< "q\n"
			<< "`\n"
			<< "b\n"
			<< "0\n";
		f.close();
	}
}
int chartoint(char character) {
	if (character >= 'a' && character <= 'z'){
		return character - 'a' + 1;
	}
	else if (character >= 'A' && character <= 'Z') {
		return character - 'A' + 1;
	}
	else if (character >= '0' && character <= '9') {
		return character - '0' + 27;
	}
	else if (character == '`') {
		return 60;
	}
}
void read_config(const std::string& filename) {
	try {
		std::ifstream f(filename);
		if (!f.is_open()) {
			std::cerr << "Could not open the file." << std::endl;
		}
		if (f.is_open()) {
			char b11;
			char b22;
			char quitbutton1;
			char restartbutton1;
			char bpmbutton1;

				f >> b11 >> b22 >> bpm >> punishmentms >> quitbutton1 >> restartbutton1 >> bpmbutton1 >> disablealt >> consolemovex >> consolemovey >> consolesizex >> consolesizey >> inputmovex >> inputmovey >> inputsizex >> inputsizey;
				b1 = chartoint(b11);
				b2 = chartoint(b22);
				quitbutton = chartoint(quitbutton1);
				restartbutton = chartoint(restartbutton1);
				bpmbutton = chartoint(bpmbutton1);

				std::cout << "Read variables: " << b1 << ", " << b2 << std::endl;
				f.close();
		}
		else {
			std::cerr << "Error reading config: "  << std::endl;
			create_default_config(filename);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error reading config: " << e.what() << std::endl;
	}
}

int main()
{
	system("color 3");
	read_config(config_filename);

	HWND console = GetConsoleWindow();
	SetWindowPos(console, HWND_TOPMOST, consolemovex, consolemovey, consolesizex, consolesizey, SWP_NOREPOSITION);
	al_init();

	ALLEGRO_DISPLAY* display = al_create_display(200, 200);
	
	al_install_keyboard();

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

	ALLEGRO_EVENT_SOURCE* keyboard_event_source = al_get_keyboard_event_source();

	al_register_event_source(event_queue, keyboard_event_source);

	ALLEGRO_COLOR color = al_map_rgb(50, 50, 50);

	al_clear_to_color(al_map_rgb(0, 0, 0));


	HWND hWnd = al_get_win_window_handle(display);
	SetWindowPos(hWnd, HWND_TOPMOST, inputmovex, inputmovey, inputsizex, inputsizex, SWP_NOREPOSITION);
	al_init_primitives_addon();

	al_flip_display();

	while (true) {
		if (exitt==true) {
			break;
		}
		
		float time_window = (60.0f / (bpm * 4.0f)) * 1000.0f;
		std::cout << "BPM is " << bpm << ". You have " << time_window << " ms between each press. Do not press one button twice. Buttons - "<< b1 << " " << b2 << std::endl;
		while (true) {
			auto begin = std::chrono::high_resolution_clock::now();

			while (true) {

				ALLEGRO_EVENT event;

				al_wait_for_event(event_queue, &event);

				if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
					
					if (event.keyboard.keycode == b1) {
						auto end = std::chrono::high_resolution_clock::now();
						float ms = float(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

						al_draw_filled_rectangle(0, 0, inputsizey / 2, inputsizey, color);
						al_draw_filled_rectangle(inputsizex / 2, 0, inputsizex, inputsizey, al_map_rgb(0, 0, 0));
						al_flip_display();
						if (start == true && ms > time_window) {
							std::cout << " -	" << " ms " << int(ms - time_window) << std::endl;
							error++;
							if (ms - time_window >= punishmentms || error >= 3) {
								std::cout << "Too late. Clicks: " << clicks << std::endl;
								goto end_loop;
							}
						}
						else {
							std::cout << " + " << int(time_window - ms) << " ms " << std::endl;
							error = 0;
						}

						start = true;
						clicks++;
						if (!disablealt){
							alternate += 1;
							alternate2 = 0;
						}

						if (alternate > 1) {
							std::cout << "Error alt. Clicks: " << clicks << std::endl;
							goto end_loop;
						}
						break;
					}

					else if (event.keyboard.keycode == b2) {
						auto end = std::chrono::high_resolution_clock::now();
						float ms = float(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
						
						al_draw_filled_rectangle(0, 0, inputsizey / 2, inputsizey, al_map_rgb(0, 0, 0));
						al_draw_filled_rectangle(inputsizex / 2, 0, inputsizex, inputsizey, color);
						al_flip_display();
						if (start == true && ms > time_window) {
							std::cout << " -	" << " ms " << int(ms - time_window) << std::endl;

							std::cout << std::endl;

							if (ms - time_window >= punishmentms || error >= 3) {
								std::cout << "Too late. Clicks: " << clicks << std::endl;
								goto end_loop;
							}
						}
						else {
							std::cout << " + " << int(time_window - ms) << " ms " << std::endl;
							error = 0;
						}
						start = true;
						clicks++;
						if (!disablealt) {
							alternate2 += 1;
							alternate = 0;
						}
						if (alternate2 > 1) {
							std::cout << "Error alt. Clicks: " << clicks << std::endl;
							goto end_loop;
						}
						break;
					}
					else {
						std::cout << "wrong key" << std::endl;
						break;
					}
				}
			}
		}


	end_loop:
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_flip_display();
		std::cout << "Q to quit, R to restart, B to change bpm" << std::endl;
		while (true) {
			ALLEGRO_EVENT event;
			al_wait_for_event(event_queue, &event);
			if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (event.keyboard.keycode == restartbutton) {
					clicks = 0;
					alternate = 0;
					alternate2 = 0;
					start = false;
					system("clear");
					break;
				}
				if (event.keyboard.keycode == quitbutton) {
					exitt = true;
					break;
				}
				if (event.keyboard.keycode == bpmbutton) {
					clicks = 0;
					alternate = 0;
					alternate2 = 0;
					start = false;
					system("clear");
					std::cout << "Click on console window to enter BPM: ";
					std::cin >> bpm;

					std::fstream file(config_filename);
					for (int i = 1; i < 3; ++i) {
						file.ignore(32767, '\n');
					}
					file.seekp(file.tellg());
					file << bpm;

					file.close();


					std::cout << std::endl << "Saved: " << bpm << std::endl;
					break;
				}
			}
		}
	}
	al_uninstall_keyboard();
}

