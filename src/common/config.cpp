// GB Enhanced+ Copyright Daniel Baxter 2014
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : config.cpp
// Date : September 20, 2014
// Description : GBE+ configuration options
//
// Parses command-line arguments to configure GBE options

#include <iostream>
#include <fstream>

#include <cstdlib>

#include "config.h"
#include "cgfx_common.h"
#include "util.h"

namespace config
{
	std::string rom_file = "";
	std::string bios_file = "";
	std::string save_file = "";
	std::string dmg_bios_path = "";
	std::string gbc_bios_path = "";
	std::string agb_bios_path = "";
	std::string save_path = "";
	std::string ss_path = "";
	std::string cfg_path = "";
	std::string data_path = "";
	std::string cheats_path = "";
	std::string external_camera_file = "";
	std::vector <std::string> recent_files;
	std::vector <std::string> cli_args;
	bool use_debugger = false;

	//Default keyboard bindings - GBA
	//Arrow Z = A button, X = B button, START = Return, Select = Space
	//UP, LEFT, DOWN, RIGHT = Arrow keys
	//A key = Left Shoulder, S key = Right Shoulder
	int agb_key_a = SDLK_z; int agb_key_b = SDLK_x; int agb_key_start = SDLK_RETURN; int agb_key_select = SDLK_SPACE;
	int agb_key_l_trigger = SDLK_a; int agb_key_r_trigger = SDLK_s;
	int agb_key_left = SDLK_LEFT; int agb_key_right = SDLK_RIGHT; int agb_key_down = SDLK_DOWN; int agb_key_up = SDLK_UP;

	//Default joystick bindings - GBA
	int agb_joy_a = 100; int agb_joy_b = 101; int agb_joy_start = 107; int agb_joy_select = 106;
	int agb_joy_r_trigger = 105; int agb_joy_l_trigger = 104;
	int agb_joy_left = 200; int agb_joy_right = 201; int agb_joy_up = 202; int agb_joy_down = 203;

	//Default keyboard bindings - DMG
	//Arrow Z = A button, X = B button, START = Return, Select = Space
	//UP, LEFT, DOWN, RIGHT = Arrow keys
	int dmg_key_a = SDLK_z; int dmg_key_b = SDLK_x; int dmg_key_start = SDLK_RETURN; int dmg_key_select = SDLK_SPACE; 
	int dmg_key_left = SDLK_LEFT; int dmg_key_right = SDLK_RIGHT; int dmg_key_down = SDLK_DOWN; int dmg_key_up = SDLK_UP;

	//Default joystick bindings - DMG
	int dmg_joy_a = 100; int dmg_joy_b = 101; int dmg_joy_start = 107; int dmg_joy_select = 106;
	int dmg_joy_left = 200; int dmg_joy_right = 201; int dmg_joy_up = 202; int dmg_joy_down = 203;

	//Default keyboard bindings - Context
	//Left = 4 (numpad), Right = 6 (numpad), Up = 8 (numpad), Down = 2 (numpad)
	int con_key_left = 260; int con_key_right = 262; int con_key_up = 264; int con_key_down = 258;

	//Default joystick bindings - Context
	int con_joy_left = 204; int con_joy_right = 205; int con_joy_up = 206; int con_joy_down = 207;

	//Hotkey bindings
	//Turbo = TAB
	int hotkey_turbo = SDLK_TAB;
	int hotkey_mute = SDLK_m;
	int hotkey_camera = SDLK_p;

	//Default joystick dead-zone
	int dead_zone = 16000;

	//Default joystick ID
	int joy_id = 0;

	//Default Haptic setting
	bool use_haptics = false;

	u32 flags = 0x4;
	bool pause_emu = false;
	bool use_bios = false;
	special_cart_types cart_type = NORMAL_CART;
	gba_save_types agb_save_type = AGB_AUTO_DETECT;
	u32 sio_device = 0;
	bool use_opengl = false;
	bool turbo = false;

	std::string vertex_shader = "vertex.vs";
	std::string fragment_shader = "fragment.fs";

	u8 scaling_factor = 1;
	u8 old_scaling_factor = 1;

	std::stringstream title;

	//Cheats - Gameshark and Game Genie
	bool use_cheats = false;
	std::vector <u32> gs_cheats;
	std::vector <std::string> gg_cheats;
	std::vector <std::string> cheats_info;

	//Patches
	bool use_patches = false;

	//Netplay settings
	bool use_netplay = true;
	bool netplay_hard_sync = true;
	u16 netplay_server_port = 2000;
	u16 netplay_client_port = 2001;
	std::string netplay_client_ip = "127.0.0.1";

	u8 dmg_gbc_pal = 0;

	//Emulated Gameboy type
	//TODO - Make this an enum
	//0 - DMG, 1 - DMG on GBC, 2 - GBC, 3 - GBA, 4 - NDS????
	u8 gb_type = 0;

	//Boolean dictating whether this is a DMG/GBC game on a GBA
	bool gba_enhance = false;

	//Variables dictating whether or not to stretch DMG/GBC games when playing on a GBA
	bool request_resize = false;
	s8 resize_mode = 0;

	//Aspect ratio
	bool maintain_aspect_ratio = false;

	//Sound parameters
	u8 volume = 128;
	u8 old_volume = 0;
	double sample_rate = 44100.0;
	bool mute = false;

	//System screen sizes
	u32 sys_width = 0;
	u32 sys_height = 0;

	//Window screen sizes
	s32 win_width = 0;
	s32 win_height = 0;

	bool sdl_render = true;

	bool use_external_interfaces = false;

	void (*render_external_sw)(std::vector<u32>&);
	void (*render_external_hw)(SDL_Surface*);
	void (*debug_external)();

	//Default Gameboy BG palettes
	u32 DMG_BG_PAL[4] = { 0xFFFFFFFF, 0xFFC0C0C0, 0xFF606060, 0xFF000000 };

	u32 DMG_OBJ_PAL[4][2] = 
	{ 
		{ 0xFFFFFFFF, 0xFFFFFFFF },
		{ 0xFFC0C0C0, 0xFFC0C0C0 },
		{ 0xFF606060, 0xFF606060 },
		{ 0xFF000000, 0xFF000000 }
	};

	//Real-time clock offsets
	u16 rtc_offset[6] = { 0, 0, 0, 0, 0, 0 };
}

/****** Reset DMG default colors ******/
void reset_dmg_colors()
{
	config::DMG_BG_PAL[0] = 0xFFFFFFFF;
	config::DMG_BG_PAL[1] = 0xFFC0C0C0;
	config::DMG_BG_PAL[2] = 0xFF606060;
	config::DMG_BG_PAL[3] = 0xFF000000;

	config::DMG_OBJ_PAL[0][0] = 0xFFFFFFFF;
	config::DMG_OBJ_PAL[1][0] = 0xFFC0C0C0;
	config::DMG_OBJ_PAL[2][0] = 0xFF606060;
	config::DMG_OBJ_PAL[3][0] = 0xFF000000;

	config::DMG_OBJ_PAL[0][1] = 0xFFFFFFFF;
	config::DMG_OBJ_PAL[1][1] = 0xFFC0C0C0;
	config::DMG_OBJ_PAL[2][1] = 0xFF606060;
	config::DMG_OBJ_PAL[3][1] = 0xFF000000;
}

/****** Set DMG colors based on GBC BIOS ******/
void set_dmg_colors(u8 color_type)
{
	switch(color_type)
	{
		//Default palette
		case 0:
			reset_dmg_colors();
			break;

		//No input
		case 1:
			config::DMG_BG_PAL[0] = 0xFFF8F8F8;
			config::DMG_BG_PAL[1] = 0xFF78F830;
			config::DMG_BG_PAL[2] = 0xFF0060C0;
			config::DMG_BG_PAL[3] = 0xFF000000;

			config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
			config::DMG_OBJ_PAL[1][0] = 0xFFF88080;
			config::DMG_OBJ_PAL[2][0] = 0xFF903838;
			config::DMG_OBJ_PAL[3][0] = 0xFF000000;

			config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
			config::DMG_OBJ_PAL[1][1] = 0xFFF88080;
			config::DMG_OBJ_PAL[2][1] = 0xFF903838;
			config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//UP
		case 2:
			config::DMG_BG_PAL[0] = 0xFFF8F8F8;
			config::DMG_BG_PAL[1] = 0xFFF8A860;
			config::DMG_BG_PAL[2] = 0xFF803000;
			config::DMG_BG_PAL[3] = 0xFF000000;

			config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
			config::DMG_OBJ_PAL[1][0] = 0xFFF8A860;
			config::DMG_OBJ_PAL[2][0] = 0xFF803000;
			config::DMG_OBJ_PAL[3][0] = 0xFF000000;

			config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
			config::DMG_OBJ_PAL[1][1] = 0xFFF8A860;
			config::DMG_OBJ_PAL[2][1] = 0xFF803000;
			config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//DOWN
		case 3:
			config::DMG_BG_PAL[0] = 0xFFF8F8A0;
			config::DMG_BG_PAL[1] = 0xFFF89090;
			config::DMG_BG_PAL[2] = 0xFF9090F8;
			config::DMG_BG_PAL[3] = 0xFF000000;

			config::DMG_OBJ_PAL[0][0] = 0xFFF8F8A0;
			config::DMG_OBJ_PAL[1][0] = 0xFFF89090;
			config::DMG_OBJ_PAL[2][0] = 0xFF9090F8;
			config::DMG_OBJ_PAL[3][0] = 0xFF000000;

			config::DMG_OBJ_PAL[0][1] = 0xFFF8F8A0;
			config::DMG_OBJ_PAL[1][1] = 0xFFF89090;
			config::DMG_OBJ_PAL[2][1] = 0xFF9090F8;
			config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//LEFT
		case 4:
			config::DMG_BG_PAL[0] = 0xFFF8F8F8;
			config::DMG_BG_PAL[1] = 0xFF60A0F8;
			config::DMG_BG_PAL[2] = 0xFF0000F8;
			config::DMG_BG_PAL[3] = 0xFF000000;

			config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
			config::DMG_OBJ_PAL[1][0] = 0xFFF88080;
			config::DMG_OBJ_PAL[2][0] = 0xFF903838;
			config::DMG_OBJ_PAL[3][0] = 0xFF000000;

			config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
			config::DMG_OBJ_PAL[1][1] = 0xFF78F830;
			config::DMG_OBJ_PAL[2][1] = 0xFF008000;
			config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//RIGHT
		case 5:
			config::DMG_BG_PAL[0] = 0xFFF8F8F8;
			config::DMG_BG_PAL[1] = 0xFF50F800;
			config::DMG_BG_PAL[2] = 0xFFF84000;
			config::DMG_BG_PAL[3] = 0xFF000000;

			config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
			config::DMG_OBJ_PAL[1][0] = 0xFF50F800;
			config::DMG_OBJ_PAL[2][0] = 0xFFF84000;
			config::DMG_OBJ_PAL[3][0] = 0xFF000000;

			config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
			config::DMG_OBJ_PAL[1][1] = 0xFF50F800;
			config::DMG_OBJ_PAL[2][1] = 0xFFF84000;
			config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//UP + A
		case 6:
                        config::DMG_BG_PAL[0] = 0xFFF8F8F8;
                        config::DMG_BG_PAL[1] = 0xFFF88080;
                        config::DMG_BG_PAL[2] = 0xFF903838;
                        config::DMG_BG_PAL[3] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][0] = 0xFF78F830;
                        config::DMG_OBJ_PAL[2][0] = 0xFF008000;
                        config::DMG_OBJ_PAL[3][0] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][1] = 0xFF60A0F8;
                        config::DMG_OBJ_PAL[2][1] = 0xFF0000F8;
                        config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//DOWN + A
		case 7:
                        config::DMG_BG_PAL[0] = 0xFFF8F8F8;
                        config::DMG_BG_PAL[1] = 0xFFF8F800;
                        config::DMG_BG_PAL[2] = 0xFFF80000;
                        config::DMG_BG_PAL[3] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][0] = 0xFFF8F800;
                        config::DMG_OBJ_PAL[2][0] = 0xFFF80000;
                        config::DMG_OBJ_PAL[3][0] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][1] = 0xFFF8F800;
                        config::DMG_OBJ_PAL[2][1] = 0xFFF80000;
                        config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//LEFT + A
		case 8:
                        config::DMG_BG_PAL[0] = 0xFFF8F8F8;
                        config::DMG_BG_PAL[1] = 0xFF8888D8;
                        config::DMG_BG_PAL[2] = 0xFF505088;
                        config::DMG_BG_PAL[3] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][0] = 0xFFF88080;
                        config::DMG_OBJ_PAL[2][0] = 0xFF903838;
                        config::DMG_OBJ_PAL[3][0] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][1] = 0xFFF8A860;
                        config::DMG_OBJ_PAL[2][1] = 0xFF803000;
                        config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//RIGHT + A
		case 9:
                        config::DMG_BG_PAL[0] = 0xFFF8F8F8;
                        config::DMG_BG_PAL[1] = 0xFF78F830;
                        config::DMG_BG_PAL[2] = 0xFF0060C0;
                        config::DMG_BG_PAL[3] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][0] = 0xFFF88080;
                        config::DMG_OBJ_PAL[2][0] = 0xFF903838;
                        config::DMG_OBJ_PAL[3][0] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][1] = 0xFFF88080;
                        config::DMG_OBJ_PAL[2][1] = 0xFF903838;
                        config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//UP + B
		case 10:
                        config::DMG_BG_PAL[0] = 0xFFF8E0C0;
                        config::DMG_BG_PAL[1] = 0xFFC89880;
                        config::DMG_BG_PAL[2] = 0xFF806828;
                        config::DMG_BG_PAL[3] = 0xFF583008;

                        config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][0] = 0xFFF8A860;
                        config::DMG_OBJ_PAL[2][0] = 0xFF803000;
                        config::DMG_OBJ_PAL[3][0] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][1] = 0xFFF8A860;
                        config::DMG_OBJ_PAL[2][1] = 0xFF803000;
                        config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//DOWN + B
		case 11:
                        config::DMG_BG_PAL[0] = 0xFFF8F8F8;
                        config::DMG_BG_PAL[1] = 0xFFF8F800;
                        config::DMG_BG_PAL[2] = 0xFF784800;
                        config::DMG_BG_PAL[3] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][0] = 0xFF60A0F8;
                        config::DMG_OBJ_PAL[2][0] = 0xFF0000F8;
                        config::DMG_OBJ_PAL[3][0] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][1] = 0xFF78F830;
                        config::DMG_OBJ_PAL[2][1] = 0xFF008000;
                        config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//LEFT + B
		case 12:
                        config::DMG_BG_PAL[0] = 0xFFF8F8F8;
                        config::DMG_BG_PAL[1] = 0xFFA0A0A0;
                        config::DMG_BG_PAL[2] = 0xFF505050;
                        config::DMG_BG_PAL[3] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][0] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][0] = 0xFFA0A0A0;
                        config::DMG_OBJ_PAL[2][0] = 0xFF505050;
                        config::DMG_OBJ_PAL[3][0] = 0xFF000000;

                        config::DMG_OBJ_PAL[0][1] = 0xFFF8F8F8;
                        config::DMG_OBJ_PAL[1][1] = 0xFFA0A0A0;
                        config::DMG_OBJ_PAL[2][1] = 0xFF505050;
                        config::DMG_OBJ_PAL[3][1] = 0xFF000000;

			break;

		//RIGHT + B
		case 13:
                        config::DMG_BG_PAL[0] = 0xFF000000;
                        config::DMG_BG_PAL[1] = 0xFF008080;
                        config::DMG_BG_PAL[2] = 0xFFF8D800;
                        config::DMG_BG_PAL[3] = 0xFFF8F8F8;

                        config::DMG_OBJ_PAL[0][0] = 0xFF000000;
                        config::DMG_OBJ_PAL[1][0] = 0xFF008080;
                        config::DMG_OBJ_PAL[2][0] = 0xFFF8D800;
                        config::DMG_OBJ_PAL[3][0] = 0xFFF8F8F8;

                        config::DMG_OBJ_PAL[0][1] = 0xFF000000;
                        config::DMG_OBJ_PAL[1][1] = 0xFF008080;
                        config::DMG_OBJ_PAL[2][1] = 0xFFF8D800;
                        config::DMG_OBJ_PAL[3][1] = 0xFFF8F8F8;

			break;

		//Classic Green
		case 14:
                        config::DMG_BG_PAL[3] = 0xFF091921;
                        config::DMG_BG_PAL[2] = 0xFF316B52;
                        config::DMG_BG_PAL[1] = 0xFF8AC573;
                        config::DMG_BG_PAL[0] = 0xFFE6FFD6;

                        config::DMG_OBJ_PAL[3][0] = 0xFF091921;
                        config::DMG_OBJ_PAL[2][0] = 0xFF316B52;
                        config::DMG_OBJ_PAL[1][0] = 0xFF8AC573;
                        config::DMG_OBJ_PAL[0][0] = 0xFFE6FFD6;

                        config::DMG_OBJ_PAL[3][1] = 0xFF091921;
                        config::DMG_OBJ_PAL[2][1] = 0xFF316B52;
                        config::DMG_OBJ_PAL[1][1] = 0xFF8AC573;
                        config::DMG_OBJ_PAL[0][1] = 0xFFE6FFD6;

			break;
	}
}	

/****** Validates emulated system type ******/
void validate_system_type()
{
	if(config::rom_file.empty()) { return; }
	if((config::rom_file == "-h") || (config::rom_file == "--help")) { config::cli_args.push_back(config::rom_file); return; } 

	//Determine Gameboy type based on file name
	//Note, DMG and GBC games are automatically detected in the Gameboy MMU, so only check for GBA types here
	std::size_t dot = config::rom_file.find_last_of(".");
	
	if(dot == std::string::npos)
	{
		std::cout<<"GBE::Warning - Could not determine the emulated system type for file " << config::rom_file << "\n";
		return;
	}

	std::string ext = config::rom_file.substr(dot);

	if(ext == ".gba") { config::gb_type = 3; }

	//Force GBC mode if system type is set to GBA, but a GB/GBC game is loaded
	else if((ext != ".gba") && (config::gb_type == 3)) 
	{
		config::gb_type = 2;
		config::gba_enhance = true;
	}
}

/****** Returns the emulated system type from a given filename ******/
u8 get_system_type_from_file(std::string filename)
{
	//Determine Gameboy type based on file name
	std::size_t dot = filename.find_last_of(".");

	if(dot == std::string::npos) { return 0; }

	std::string ext = filename.substr(dot);
	
	u8 gb_type = config::gb_type;

	if(ext == ".gba") { gb_type = 3; }
	else if((ext != ".gba") && (gb_type == 3)) { gb_type = 2; }

	//For Auto or GBC mode, determine what the CGB Flag is
	if((gb_type == 0) || (gb_type == 2))
	{
		std::ifstream test_stream(filename.c_str(), std::ios::binary);
		
		if(test_stream.is_open())
		{
			u8 color_byte;

			test_stream.seekg(0x143);
			test_stream.read((char*)&color_byte, 1);

			//If GBC compatible, use GBC mode. Otherwise, use DMG mode
			if((color_byte == 0xC0) || (color_byte == 0x80)) { gb_type = 2; }
			else { gb_type = 1; }

			test_stream.close();
		}
	}

	return gb_type;
}

/****** Parse arguments passed from the command-line ******/
bool parse_cli_args()
{
	//If no arguments were passed, cannot run without ROM file
	//If using external interfaces (e.g. the GUI), a ROM file is not necessary
	if((config::cli_args.size() < 1) && (!config::use_external_interfaces))
	{
		std::cout<<"GBE::Error - No ROM file in arguments \n";
		return false;
	}

	else 
	{
		//Parse the rest of the arguments if any		
		for(int x = 1; x < config::cli_args.size(); x++)
		{	
			//Run GBE+ in debug mode
			if((config::cli_args[x] == "-d") || (config::cli_args[x] == "--debug")) { config::use_debugger = true; }

			//Load BIOS
			else if((config::cli_args[x] == "-b") || (config::cli_args[x] == "--bios")) 
			{
				if((++x) == config::cli_args.size()) { std::cout<<"GBE::Error - No BIOS file in arguments\n"; }

				else 
				{ 
					config::use_bios = true; 
					config::bios_file = config::cli_args[x];
				}
			}

			//Enable fullscreen mode
			else if((config::cli_args[x] == "-f") || (config::cli_args[x] == "--fullscreen")) { config::flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; } 

			//Use MBC1M multicart mode if applicable for a given ROM
			else if(config::cli_args[x] == "--mbc1m") { config::cart_type = DMG_MBC1M; }

			//Use MMM01 multicart mode if applicable for a given ROM
			else if(config::cli_args[x] == "--mmm01") { config::cart_type = DMG_MMM01; }

			//Use Auto-Detect for GBA saves
			else if(config::cli_args[x] == "--save-auto") { config::agb_save_type = AGB_AUTO_DETECT; }

			//Disable all GBA saves
			else if(config::cli_args[x] == "--save-none") { config::agb_save_type = AGB_NO_SAVE; }

			//Force SRAM GBA saves
			else if(config::cli_args[x] == "--save-sram") { config::agb_save_type = AGB_SRAM; }

			//Force EEPROM GBA saves
			else if(config::cli_args[x] == "--save-eeprom") { config::agb_save_type = AGB_EEPROM; }

			//Force FLASH 64KB GBA saves
			else if(config::cli_args[x] == "--save-flash64") { config::agb_save_type = AGB_FLASH64; }

			//Force FLASH 128KB GBA saves
			else if(config::cli_args[x] == "--save-auto") { config::agb_save_type = AGB_FLASH128; }

			//Use OpenGL for screen drawing
			else if(config::cli_args[x] == "--opengl") { config::use_opengl = true; }

			//Use Gameshark or Game Genie cheats
			else if(config::cli_args[x] == "--cheats") { config::use_cheats = true; }

			else if(config::cli_args[x] == "--patch") { config::use_patches = true; }

			//Scale screen by 2x
			else if(config::cli_args[x] == "--2x") { config::scaling_factor = config::old_scaling_factor = 2; }

			//Scale screen by 3x
			else if(config::cli_args[x] == "--3x") { config::scaling_factor = config::old_scaling_factor = 3; }

			//Scale screen by 4x
			else if(config::cli_args[x] == "--4x") { config::scaling_factor = config::old_scaling_factor = 4; }

			//Scale screen by 5x
			else if(config::cli_args[x] == "--5x") { config::scaling_factor = config::old_scaling_factor = 5; }

			//Scale screen by 6x
			else if(config::cli_args[x] == "--6x") { config::scaling_factor = config::old_scaling_factor = 6; }

			//Set system type - Auto
			else if(config::cli_args[x] == "--sys-auto") { config::gb_type = 0; }

			//Set system type - DMG
			else if(config::cli_args[x] == "--sys-dmg") { config::gb_type = 1; }

			//Set system type - GBC
			else if(config::cli_args[x] == "--sys-gbc") { config::gb_type = 2; }

			//Set system type - GBA
			else if(config::cli_args[x] == "--sys-gba") { config::gb_type = 3; }

			//Print Help
			else if((config::cli_args[x] == "-h") || (config::cli_args[x] == "--help")) 
			{
				if(!config::use_external_interfaces) { std::cout<<"\ngbe_plus file [options ...]\n\n"; }
				else { std::cout<<"\ngbe_plus_qt file [options ...]\n\n"; }

				std::cout<<"GBE+ Command Line Options:\n";
				std::cout<<"-b [FILE], --bios [FILE] \t\t Load and use BIOS file\n";
				std::cout<<"-d, --debug \t\t\t\t Start the command-line debugger\n";
				std::cout<<"--mbc1m \t\t\t\t Use MBC1M multicart mode if applicable\n";
				std::cout<<"--mmm01 \t\t\t\t Use MMM01 multicart mode if applicable\n";
				std::cout<<"--opengl \t\t\t\t Use OpenGL for screen drawing and scaling\n";
				std::cout<<"--cheats \t\t\t\t Use Gameshark or Game Genie cheats\n";
				std::cout<<"--patch \t\t\t\t Use a patch file for the ROM\n";
				std::cout<<"--2x, --3x, --4x, --5x, --6x \t\t Scale screen by a given factor (OpenGL only)\n";
				std::cout<<"--sys-auto \t\t\t\t Set the emulated system type to AUTO\n";
				std::cout<<"--sys-dmg \t\t\t\t Set the emulated system type to DMG (old Gameboy)\n";
				std::cout<<"--sys-gbc \t\t\t\t Set the emulated system type to GBC\n";
				std::cout<<"--sys-gba \t\t\t\t Set the emulated system type to GBA\n";
				std::cout<<"--save-auto \t\t\t\t Set the GBA save type to Auto Detect\n";
				std::cout<<"--save-none \t\t\t\t Disables all GBA saves\n";
				std::cout<<"--save-sram \t\t\t\t Force the GBA save type to SRAM\n";
				std::cout<<"--save-eeprom \t\t\t\t Force the GBA save type to EEPROM\n";
				std::cout<<"--save-flash64 \t\t\t\t Force the GBA save type to FLASH 64KB\n";
				std::cout<<"--save-flash128 \t\t\t Force the GBA save type to FLASH 128KB\n";
				std::cout<<"-h, --help \t\t\t\t Print these help messages\n";
				return false;
			}

			else
			{
				std::cout<<"GBE::Error - Unknown argument - " << config::cli_args[x] << "\n";
				return false;
			}
		}

		return true;
	}
}

/****** Parse ROM filename and save file ******/
void parse_filenames()
{
	//ROM file is always first argument
	config::rom_file = config::cli_args[0];
	config::save_file = config::rom_file + ".sav";

	validate_system_type();
}

/****** Parse options from the .ini file ******/
bool parse_ini_file()
{
	//Test for Windows or Portable version first
	//Always give preference to portable .ini settings on every OS
	std::ifstream file("gbe.ini", std::ios::in);
	config::data_path = "./data/";

	std::string input_line = "";
	std::string line_char = "";

	//Clear recent files
	config::recent_files.clear();

	//Clear existing .ini parameters
	std::vector <std::string> ini_opts;
	ini_opts.clear();

	if(!file.is_open())
	{
		std::string unix_str = getenv("HOME");
		std::string last_chr = "";

		last_chr = unix_str[unix_str.length() - 1];
		config::cfg_path = (last_chr == "/") ? unix_str + ".gbe_plus/" : unix_str + "/.gbe_plus/";
		config::data_path = config::cfg_path + "data/";
		unix_str += (last_chr == "/") ? ".gbe_plus/gbe.ini" : "/.gbe_plus/gbe.ini";

		//Test for Linux or Unix install location next
		file.open(unix_str.c_str(), std::ios::in);
		
		if(!file.is_open())
		{
			std::cout<<"GBE::Error - Could not open gbe.ini configuration file. Check file path or permissions. \n";
			return false;
		} 
	}

	//Cycle through whole file, line-by-line
	while(getline(file, input_line))
	{
		line_char = input_line[0];
		bool ignore = false;	
	
		//Check if line starts with [ - if not, skip line
		if(line_char == "[")
		{
			std::string line_item = "";

			//Cycle through line, character-by-character
			for(int x = 0; ++x < input_line.length();)
			{
				line_char = input_line[x];

				//Check for single-quotes, don't parse ":" or "]" within them
				if((line_char == "'") && (!ignore)) { ignore = true; }
				else if((line_char == "'") && (ignore)) { ignore = false; }

				//Check the character for item limiter : or ] - Push to Vector
				else if(((line_char == ":") || (line_char == "]")) && (!ignore)) 
				{
					ini_opts.push_back(line_item);
					line_item = ""; 
				}

				else { line_item += line_char; }
			}
		}
	}
	
	file.close();

	//Cycle through all items in the .ini file
	//Set options as appropiate
	int size = ini_opts.size();
	int output = 0;
	std::string ini_item = "";

	for(int x = 0; x < size; x++)
	{
		ini_item = ini_opts[x];

		//Use BIOS
		if(ini_item == "#use_bios")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { config::use_bios = true; }
				else { config::use_bios = false; }
			}

			else 
			{ 
				std::cout<<"GBE::Error - Could not parse gbe.ini (#use_bios) \n";
				return false;
			}
		}

		//Emulated SIO device
		if(ini_item == "#sio_device")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 0) && (output <= 3)) { config::sio_device = output; }
			}

			else 
			{ 
				std::cout<<"GBE::Error - Could not parse gbe.ini (#sio_device) \n";
				return false;
			}
		}

		//Set emulated system type
		else if(ini_item == "#system_type")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 0) && (output <= 3)) 
				{
					config::gb_type = output;
					validate_system_type();
				}
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#system_type) \n";
				return false;
			}
		}

		//Use cheats
		if(ini_item == "#use_cheats")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { config::use_cheats = true; }
				else { config::use_cheats = false; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#use_cheats) \n";
				return false;
			}
		}

		//Use patches
		if(ini_item == "#use_patches")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { config::use_patches = true; }
				else { config::use_patches = false; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#use_patches) \n";
				return false;
			}
		}

		//DMG BIOS path
		else if(ini_item == "#dmg_bios_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::dmg_bios_path = ini_item; }
				else { config::dmg_bios_path = ""; x--;}
 
			}

			else { config::dmg_bios_path = ""; }
		}

		//GBC BIOS path
		else if(ini_item == "#gbc_bios_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::gbc_bios_path = ini_item; }
				else { config::gbc_bios_path = ""; x--;}
 
			}

			else { config::gbc_bios_path = ""; }
		}

		//GBA BIOS path
		else if(ini_item == "#agb_bios_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::agb_bios_path = ini_item; }
				else { config::agb_bios_path = ""; x--;}
 
			}

			else { config::agb_bios_path = ""; }
		}

		//Game save path
		else if(ini_item == "#save_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::save_path = ini_item; }
				else { config::save_path = ""; x--;}
 
			}

			else { config::save_path = ""; }
		}

		//Screenshots path
		else if(ini_item == "#screenshot_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::ss_path = ini_item; }
				else { config::ss_path = ""; x--;}
 
			}

			else { config::ss_path = ""; }
		}

		//Cheats path
		else if(ini_item == "#cheats_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::cheats_path = ini_item; }
				else { config::cheats_path = ""; x--;}
 
			}

			else { config::cheats_path = ""; }
		}

		//External camera file
		else if(ini_item == "#camera_file")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::external_camera_file = ini_item; }
				else { config::external_camera_file = ""; x--;}
 
			}

			else { config::external_camera_file = ""; }
		}

		//Use OpenGL
		else if(ini_item == "#use_opengl")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { config::use_opengl = true; }
				else { config::use_opengl = false; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#use_opengl) \n";
				return false;
			}
		}

		//Fragment shader
		else if(ini_item == "#fragment_shader")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::fragment_shader = config::data_path + "shaders/" + ini_item; }
				else { config::fragment_shader = config::data_path + "shaders/fragment.fs"; x--;}
 
			}

			else { config::fragment_shader = config::data_path + "shaders/fragment.fs"; }
		}

		//Vertex shader
		else if(ini_item == "#vertex_shader")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { config::vertex_shader = config::data_path + "shaders/" + ini_item; }
				else { config::vertex_shader = config::data_path + "shaders/vertex.vs"; x--;}
 
			}

			else { config::vertex_shader = config::data_path + "shaders/vertex.vs"; }
		}

		//Use gamepad dead zone
		else if(ini_item == "#dead_zone")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 0) && (output <= 32767)) { config::dead_zone = output; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#dead_zone) \n";
				return false;
			}
		}

		//Use haptics
		else if(ini_item == "#use_haptics")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { config::use_haptics = true; }
				else { config::use_haptics = false; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#use_haptics) \n";
				return false;
			}
		}

		//Volume settings
		else if(ini_item == "#volume")
		{
			if((x + 1) < size)
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 0) && (output <= 128)) { config::volume = output; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#volume) \n";
				return false;
			}
		}

		//Mute settings
		else if(ini_item == "#mute")
		{
			if((x + 1) < size)
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 0) && (output <= 1)) { config::mute = output; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#mute) \n";
				return false;
			}
		}

		//Sample rate
		else if(ini_item == "#sample_rate")
		{
			if((x + 1) < size)
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 1) && (output <= 48000)) { config::sample_rate = (double)output; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#sample_rate) \n";
				return false;
			}
		}

		//Scaling factor
		else if(ini_item == "#scaling_factor")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 1) && (output <= 10)) { config::scaling_factor = config::old_scaling_factor = output; }
				else { config::scaling_factor = 1; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#scaling_factor) \n";
				return false;
			}
		}

		//Maintain aspect ratio
		else if(ini_item == "#maintain_aspect_ratio")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { config::maintain_aspect_ratio = true; }
				else { config::maintain_aspect_ratio = false; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#maintain_aspect_ratio) \n";
				return false;
			}
		}

		//Real-time clock offsets
		else if(ini_item == "#rtc_offset")
		{
			if((x + 6) < size)
			{
				std::stringstream temp_stream;

				//Seconds offset
				temp_stream << ini_opts[++x];
				temp_stream >> config::rtc_offset[0];
				temp_stream.clear();
				temp_stream.str(std::string());

				if(config::rtc_offset[0] > 59) { config::rtc_offset[0] = 59; }

				//Minutes offset
				temp_stream << ini_opts[++x];
				temp_stream >> config::rtc_offset[1];
				temp_stream.clear();
				temp_stream.str(std::string());

				if(config::rtc_offset[1] > 59) { config::rtc_offset[1] = 59; }

				//Hours offset
				temp_stream << ini_opts[++x];
				temp_stream >> config::rtc_offset[2];
				temp_stream.clear();
				temp_stream.str(std::string());

				if(config::rtc_offset[2] > 23) { config::rtc_offset[2] = 23; }

				//Days offset
				temp_stream << ini_opts[++x];
				temp_stream >> config::rtc_offset[3];
				temp_stream.clear();
				temp_stream.str(std::string());

				if(config::rtc_offset[3] > 365) { config::rtc_offset[3] = 365; }

				//Months offset
				temp_stream << ini_opts[++x];
				temp_stream >> config::rtc_offset[4];
				temp_stream.clear();
				temp_stream.str(std::string());

				if(config::rtc_offset[4] > 11) { config::rtc_offset[4] = 11; }

				//Years offset
				temp_stream << ini_opts[++x];
				temp_stream >> config::rtc_offset[5];
				temp_stream.clear();
				temp_stream.str(std::string());
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#rtc_offset) \n";
				return false;
			}
		}

		//Emulated DMG-on-GBC palette
		else if(ini_item == "#dmg_on_gbc_pal")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 1) && (output <= 14)) 
				{
					config::dmg_gbc_pal = output;
					set_dmg_colors(config::dmg_gbc_pal);
				}
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#dmg_on_gbc_pal) \n";
				return false;
			}
		}

		//DMG-GBC keyboard controls
		else if(ini_item == "#dmg_key_controls")
		{
			if((x + 8) < size)
			{
				std::stringstream temp_stream;

				//A
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_key_a;
				temp_stream.clear();
				temp_stream.str(std::string());

				//B
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_key_b;
				temp_stream.clear();
				temp_stream.str(std::string());

				//START
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_key_start;
				temp_stream.clear();
				temp_stream.str(std::string());

				//SELECT
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_key_select;
				temp_stream.clear();
				temp_stream.str(std::string());

				//LEFT
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_key_left;
				temp_stream.clear();
				temp_stream.str(std::string());

				//RIGHT
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_key_right;
				temp_stream.clear();
				temp_stream.str(std::string());

				//UP
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_key_up;
				temp_stream.clear();
				temp_stream.str(std::string());

				//DOWN
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_key_down;
				temp_stream.clear();
				temp_stream.str(std::string());
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#dmg_key_controls) \n";
				return false;
			}
		}

		//DMG-GBC gamepad controls
		else if(ini_item == "#dmg_joy_controls")
		{
			if((x + 8) < size)
			{
				std::stringstream temp_stream;

				//A
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_joy_a;
				temp_stream.clear();
				temp_stream.str(std::string());

				//B
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_joy_b;
				temp_stream.clear();
				temp_stream.str(std::string());

				//START
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_joy_start;
				temp_stream.clear();
				temp_stream.str(std::string());

				//SELECT
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_joy_select;
				temp_stream.clear();
				temp_stream.str(std::string());

				//LEFT
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_joy_left;
				temp_stream.clear();
				temp_stream.str(std::string());

				//RIGHT
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_joy_right;
				temp_stream.clear();
				temp_stream.str(std::string());

				//UP
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_joy_up;
				temp_stream.clear();
				temp_stream.str(std::string());

				//DOWN
				temp_stream << ini_opts[++x];
				temp_stream >> config::dmg_joy_down;
				temp_stream.clear();
				temp_stream.str(std::string());
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#dmg_joy_controls) \n";
				return false;
			}
		}

		//GBA keyboard controls
		else if(ini_item == "#agb_key_controls")
		{
			if((x + 10) < size)
			{
				std::stringstream temp_stream;

				//A
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_a;
				temp_stream.clear();
				temp_stream.str(std::string());

				//B
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_b;
				temp_stream.clear();
				temp_stream.str(std::string());

				//START
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_start;
				temp_stream.clear();
				temp_stream.str(std::string());

				//SELECT
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_select;
				temp_stream.clear();
				temp_stream.str(std::string());

				//LEFT
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_left;
				temp_stream.clear();
				temp_stream.str(std::string());

				//RIGHT
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_right;
				temp_stream.clear();
				temp_stream.str(std::string());

				//UP
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_up;
				temp_stream.clear();
				temp_stream.str(std::string());

				//DOWN
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_down;
				temp_stream.clear();
				temp_stream.str(std::string());

				//LEFT TRIGGER
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_l_trigger;
				temp_stream.clear();
				temp_stream.str(std::string());

				//RIGHT TRIGGER
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_key_r_trigger;
				temp_stream.clear();
				temp_stream.str(std::string());
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#agb_key_controls) \n";
				return false;
			}
		}

		//GBA gamepad controls
		else if(ini_item == "#agb_joy_controls")
		{
			if((x + 10) < size)
			{
				std::stringstream temp_stream;

				//A
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_a;
				temp_stream.clear();
				temp_stream.str(std::string());

				//B
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_b;
				temp_stream.clear();
				temp_stream.str(std::string());

				//START
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_start;
				temp_stream.clear();
				temp_stream.str(std::string());

				//SELECT
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_select;
				temp_stream.clear();
				temp_stream.str(std::string());

				//LEFT
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_left;
				temp_stream.clear();
				temp_stream.str(std::string());

				//RIGHT
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_right;
				temp_stream.clear();
				temp_stream.str(std::string());

				//UP
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_up;
				temp_stream.clear();
				temp_stream.str(std::string());

				//DOWN
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_down;
				temp_stream.clear();
				temp_stream.str(std::string());

				//LEFT TRIGGER
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_l_trigger;
				temp_stream.clear();
				temp_stream.str(std::string());

				//RIGHT TRIGGER
				temp_stream << ini_opts[++x];
				temp_stream >> config::agb_joy_r_trigger;
				temp_stream.clear();
				temp_stream.str(std::string());
			}


			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#agb_joy_controls) \n";
				return false;
			}
		}

		//Context keyboard controls
		else if(ini_item == "#con_key_controls")
		{
			if((x + 4) < size)
			{
				std::stringstream temp_stream;

				//LEFT
				temp_stream << ini_opts[++x];
				temp_stream >> config::con_key_left;
				temp_stream.clear();
				temp_stream.str(std::string());

				//RIGHT
				temp_stream << ini_opts[++x];
				temp_stream >> config::con_key_right;
				temp_stream.clear();
				temp_stream.str(std::string());

				//UP
				temp_stream << ini_opts[++x];
				temp_stream >> config::con_key_up;
				temp_stream.clear();
				temp_stream.str(std::string());

				//DOWN
				temp_stream << ini_opts[++x];
				temp_stream >> config::con_key_down;
				temp_stream.clear();
				temp_stream.str(std::string());
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#con_key_controls) \n";
				return false;
			}
		}

		//Context joystick controls
		else if(ini_item == "#con_joy_controls")
		{
			if((x + 4) < size)
			{
				std::stringstream temp_stream;

				//LEFT
				temp_stream << ini_opts[++x];
				temp_stream >> config::con_joy_left;
				temp_stream.clear();
				temp_stream.str(std::string());

				//RIGHT
				temp_stream << ini_opts[++x];
				temp_stream >> config::con_joy_right;
				temp_stream.clear();
				temp_stream.str(std::string());

				//UP
				temp_stream << ini_opts[++x];
				temp_stream >> config::con_joy_up;
				temp_stream.clear();
				temp_stream.str(std::string());

				//DOWN
				temp_stream << ini_opts[++x];
				temp_stream >> config::con_joy_down;
				temp_stream.clear();
				temp_stream.str(std::string());
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#con_joy_controls) \n";
				return false;
			}
		}

		//Hotkeys
		else if(ini_item == "#hotkeys")
		{
			if((x + 3) < size)
			{
				std::stringstream temp_stream;

				//Turbo
				temp_stream << ini_opts[++x];
				temp_stream >> config::hotkey_turbo;
				temp_stream.clear();
				temp_stream.str(std::string());

				//Mute
				temp_stream << ini_opts[++x];
				temp_stream >> config::hotkey_mute;
				temp_stream.clear();
				temp_stream.str(std::string());

				//GB Camera
				temp_stream << ini_opts[++x];
				temp_stream >> config::hotkey_camera;
				temp_stream.clear();
				temp_stream.str(std::string());
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#hotkeys) \n";
				return false;
			}
		}

		//Use CGFX
		else if(ini_item == "#use_cgfx")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { cgfx::load_cgfx = true; }
				else { cgfx::load_cgfx = false; }
			}

			else 
			{ 
				std::cout<<"GBE::Error - Could not parse gbe.ini (#use_cgfx) \n";
				return false;
			}
		}

		//CGFX manifest path
		else if(ini_item == "#manifest_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { cgfx::manifest_file = ini_item; }
				else { cgfx::manifest_file = ""; x--;}
 
			}

			else { cgfx::manifest_file = ""; }
		}

		//CGFX BG Tile dump folder
		else if(ini_item == "#dump_bg_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { cgfx::dump_bg_path = ini_item; }
				else { x--; }
			}
		}

		//CGFX OBJ Tile dump folder
		else if(ini_item == "#dump_obj_path")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#") { cgfx::dump_obj_path = ini_item; }
				else { x--; }
			}
		}

		//CGFX Scaling factor
		else if(ini_item == "#cgfx_scaling_factor")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if((output >= 1) && (output <= 10)) { cgfx::scaling_factor = output; }
				else { cgfx::scaling_factor = 1; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#cgfx_scaling_factor) \n";
				return false;
			}
		}

		//CGFX Transparency color
		else if(ini_item == "#cgfx_transparency")
		{
			if((x + 1) < size)
			{
				ini_item = ini_opts[++x];
				std::size_t found = ini_item.find("0x");
				std::string format = ini_item.substr(0, 2);

				//Value must be in hex format with "0x"
				if(format != "0x")
				{
					std::cout<<"GBE::Error - Could not parse gbe.ini (#cgfx_transparency) \n";
					return false;
				}

				std::string hex_color = ini_item.substr(found + 2);

				//Value must not be more than 8 characters long for AARRGGBB
				if(hex_color.size() > 8)
				{
					std::cout<<"GBE::Error - Could not parse gbe.ini (#cgfx_transparency) \n";
					return false;
				}

				u32 transparency = 0;

				//Parse the string into hex
				if(!util::from_hex_str(hex_color, transparency))
				{
					std::cout<<"GBE::Error - Could not parse gbe.ini (#cgfx_transparency) \n";
					return false;
				}
			}

			else
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#cgfx_transparency) \n";
				return false;
			}
		}

		//Use netplay
		else if(ini_item == "#use_netplay")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { config::use_netplay = true; }
				else { config::use_netplay = false; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#use_netplay) \n";
				return false;
			}
		}

		//Use netplay hard sync
		if(ini_item == "#use_netplay_hard_sync")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output == 1) { config::netplay_hard_sync = true; }
				else { config::netplay_hard_sync = false; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#use_netplay_hard_sync) \n";
				return false;
			}
		}

		//Netplay server port
		else if(ini_item == "#netplay_server_port")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output <= 65535) { config::netplay_server_port = output; }
				else { config::netplay_server_port = 2000; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#netplay_server_port) \n";
				return false;
			}
		}

		//Netplay client port
		else if(ini_item == "#netplay_client_port")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::stringstream temp_stream(ini_item);
				temp_stream >> output;

				if(output <= 65535) { config::netplay_client_port = output; }
				else { config::netplay_client_port = 2001; }
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#netplay_client_port) \n";
				return false;
			}
		}

		//Netplay client IP address
		else if(ini_item == "#netplay_client_ip")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				config::netplay_client_ip = ini_item;
			}

			else 
			{
				std::cout<<"GBE::Error - Could not parse gbe.ini (#netplay_client_ip) \n";
				return false;
			}
		}

		//Recent files
		else if(ini_item == "#recent_files")
		{
			if((x + 1) < size) 
			{
				ini_item = ini_opts[++x];
				std::string first_char = "";
				first_char = ini_item[0];
				
				//When left blank, don't parse the next line item
				if(first_char != "#")
				{
					//Only take at most the 1st 10 entries
					if(config::recent_files.size() < 10) { config::recent_files.push_back(ini_item); }
				}

				else { x--; }
			}
		}
	}

	return true;
}

/****** Save options to the .ini file ******/
bool save_ini_file()
{
	//Test for Windows or Portable version first
	//Always give preference to portable .ini settings on every OS
	std::string ini_path = config::cfg_path + "gbe.ini";
	std::ifstream in_file(ini_path.c_str(), std::ios::in);

	std::string input_line = "";
	std::string line_char = "";

	std::vector <std::string> output_lines;
	std::vector <u32> output_count;
	int line_counter = 0;
	int recent_count = config::recent_files.size();

	//Clear existing .ini parameters
	std::vector <std::string> ini_opts;
	ini_opts.clear();

	if(!in_file.is_open())
	{
		std::cout<<"GBE::Error - Could not open gbe.ini configuration file. Check file path or permissions. \n";
		return false; 
	}

	//Cycle through whole file, line-by-line
	while(getline(in_file, input_line))
	{
		line_char = input_line[0];
		bool ignore = false;	

		//Push line to output text for later manipulation
		output_lines.push_back(input_line);
	
		//Check if line starts with [ - if not, skip line
		if(line_char == "[")
		{
			std::string line_item = "";

			//Cycle through line, character-by-character
			for(int x = 0; ++x < input_line.length();)
			{
				line_char = input_line[x];

				//Check for single-quotes, don't parse ":" or "]" within them
				if((line_char == "'") && (!ignore)) { ignore = true; }
				else if((line_char == "'") && (ignore)) { ignore = false; }

				//Check the character for item limiter : or ] - Push to Vector
				else if(((line_char == ":") || (line_char == "]")) && (!ignore)) 
				{
					ini_opts.push_back(line_item);
					output_count.push_back(line_counter);
					line_item = "";
				}

				else { line_item += line_char; }
			}
		}

		line_counter++;
	}
	
	in_file.close();

	//Cycle through all items in the .ini file
	//Save options as appropiate
	int size = ini_opts.size();
	int line_pos = 0;
	std::string ini_item = "";

	for(int x = 0; x < size; x++)
	{
		ini_item = ini_opts[x];

		//Use BIOS
		if(ini_item == "#use_bios")
		{
			line_pos = output_count[x];
			std::string val = (config::use_bios) ? "1" : "0";

			output_lines[line_pos] = "[#use_bios:" + val + "]";
		}

		//Use GB Printer
		if(ini_item == "#sio_device")
		{
			line_pos = output_count[x];

			output_lines[line_pos] = "[#sio_device:" + util::to_str(config::sio_device) + "]";
		}

		//Set emulated system type
		else if(ini_item == "#system_type")
		{
			line_pos = output_count[x];

			output_lines[line_pos] = "[#system_type:" + util::to_str(config::gb_type) + "]";
		}

		//Use cheats
		else if(ini_item == "#use_cheats")
		{
			line_pos = output_count[x];
			std::string val = (config::use_cheats) ? "1" : "0";

			output_lines[line_pos] = "[#use_cheats:" + val + "]";
		}

		//Use patches
		else if(ini_item == "#use_patches")
		{
			line_pos = output_count[x];
			std::string val = (config::use_patches) ? "1" : "0";

			output_lines[line_pos] = "[#use_patches:" + val + "]";
		}

		//DMG BIOS path
		else if(ini_item == "#dmg_bios_path")
		{
			line_pos = output_count[x];
			std::string val = (config::dmg_bios_path == "") ? "" : (":'" + config::dmg_bios_path + "'");

			output_lines[line_pos] = "[#dmg_bios_path" + val + "]";
		}

		//GBC BIOS path
		else if(ini_item == "#gbc_bios_path")
		{
			line_pos = output_count[x];
			std::string val = (config::gbc_bios_path == "") ? "" : (":'" + config::gbc_bios_path + "'");

			output_lines[line_pos] = "[#gbc_bios_path" + val + "]";
		}

		//GBA BIOS path
		else if(ini_item == "#agb_bios_path")
		{
			line_pos = output_count[x];
			std::string val = (config::agb_bios_path == "") ? "" : (":'" + config::agb_bios_path + "'");

			output_lines[line_pos] = "[#agb_bios_path" + val + "]";
		}

		//Game save path
		else if(ini_item == "#save_path")
		{
			line_pos = output_count[x];
			std::string val = (config::save_path == "") ? "" : (":'" + config::save_path + "'");

			output_lines[line_pos] = "[#save_path" + val + "]";
		}

		//Screenshots path
		else if(ini_item == "#screenshot_path")
		{
			line_pos = output_count[x];
			std::string val = (config::ss_path == "") ? "" : (":'" + config::ss_path + "'");

			output_lines[line_pos] = "[#screenshot_path" + val + "]";
		}

		//Cheats path
		else if(ini_item == "#cheats_path")
		{
			line_pos = output_count[x];
			std::string val = (config::cheats_path == "") ? "" : (":'" + config::cheats_path + "'");

			output_lines[line_pos] = "[#cheats_path" + val + "]";
		}

		//External camera file
		else if(ini_item == "#camera_file")
		{
			line_pos = output_count[x];
			std::string val = (config::external_camera_file == "") ? "" : (":'" + config::external_camera_file + "'");

			output_lines[line_pos] = "[#camera_file" + val + "]";
		}


		//Use OpenGL
		else if(ini_item == "#use_opengl")
		{
			line_pos = output_count[x];
			std::string val = (config::use_opengl) ? "1" : "0";

			output_lines[line_pos] = "[#use_opengl:" + val + "]";
		}

		//Use gamepad dead zone
		else if(ini_item == "#dead_zone")
		{
			line_pos = output_count[x];

			output_lines[line_pos] = "[#dead_zone:" + util::to_str(config::dead_zone) + "]";
		}

		//Use haptics
		else if(ini_item == "#use_haptics")
		{
			line_pos = output_count[x];
			std::string val = (config::use_haptics) ? "1" : "0";

			output_lines[line_pos] = "[#use_haptics:" + val + "]";
		}

		//Volume settings
		else if(ini_item == "#volume")
		{
			line_pos = output_count[x];

			output_lines[line_pos] = "[#volume:" + util::to_str(config::volume) + "]";
		}

		//Mute settings
		else if(ini_item == "#mute")
		{
			line_pos = output_count[x];
			std::string val = (config::mute) ? "1" : "0";

			output_lines[line_pos] = "[#mute:" + val + "]";
		}

		//Sample rate
		else if(ini_item == "#sample_rate")
		{
			line_pos = output_count[x];

			output_lines[line_pos] = "[#sample_rate:" + util::to_str(config::sample_rate) + "]";
		}

		//Scaling factor
		else if(ini_item == "#scaling_factor")
		{
			line_pos = output_count[x];

			output_lines[line_pos] = "[#scaling_factor:" + util::to_str(config::scaling_factor) + "]";
		}

		//Maintain aspect ratio
		else if(ini_item == "#maintain_aspect_ratio")
		{
			line_pos = output_count[x];
			std::string val = (config::maintain_aspect_ratio) ? "1" : "0";

			output_lines[line_pos] = "[#maintain_aspect_ratio:" + val + "]";
		}

		//Real-time clock offsets
		else if(ini_item == "#rtc_offset")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::rtc_offset[0]) + ":";
			val += util::to_str(config::rtc_offset[1]) + ":";
			val += util::to_str(config::rtc_offset[2]) + ":";
			val += util::to_str(config::rtc_offset[3]) + ":";
			val += util::to_str(config::rtc_offset[4]) + ":";
			val += util::to_str(config::rtc_offset[5]);

			output_lines[line_pos] = "[#rtc_offset:" + val + "]";
		}

		//Emulated DMG-on-GBC palette
		else if(ini_item == "#dmg_on_gbc_pal")
		{
			line_pos = output_count[x];

			output_lines[line_pos] = "[#dmg_on_gbc_pal:" + util::to_str(config::dmg_gbc_pal) + "]";
		}

		//OpenGL Fragment Shader
		else if(ini_item == "#fragment_shader")
		{
			line_pos = output_count[x];

			if(config::fragment_shader == (config::data_path + "shaders/fragment.fs")) { config::fragment_shader = "fragment.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/2xBR.fs")) { config::fragment_shader = "2xBR.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/4xBR.fs")) { config::fragment_shader = "4xBR.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/bad_bloom.fs")) { config::fragment_shader = "bad_bloom.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/chrono.fs")) { config::fragment_shader = "chrono.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/grayscale.fs")) { config::fragment_shader= "grayscale.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/pastel.fs")) { config::fragment_shader = "pastel.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/scale2x.fs")) { config::fragment_shader = "scale2x.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/scale3x.fs")) { config::fragment_shader = "scale3x.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/sepia.fs")) { config::fragment_shader = "sepia.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/spotlight.fs")) { config::fragment_shader = "spotlight.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/tv_mode.fs")) { config::fragment_shader = "tv_mode.fs"; }
			else if(config::fragment_shader == (config::data_path + "shaders/washout.fs")) { config::fragment_shader = "washout.fs"; }

			output_lines[line_pos] = "[#fragment_shader:'" + config::fragment_shader + "']";
		}

		//DMG-GBC keyboard controls
		else if(ini_item == "#dmg_key_controls")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::dmg_key_a) + ":";
			val += util::to_str(config::dmg_key_b) + ":";
			val += util::to_str(config::dmg_key_start) + ":";
			val += util::to_str(config::dmg_key_select) + ":";
			val += util::to_str(config::dmg_key_left) + ":";
			val += util::to_str(config::dmg_key_right) + ":";
			val += util::to_str(config::dmg_key_up) + ":";
			val += util::to_str(config::dmg_key_down);

			output_lines[line_pos] = "[#dmg_key_controls:" + val + "]";
		}

		//DMG-GBC gamepad controls
		else if(ini_item == "#dmg_joy_controls")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::dmg_joy_a) + ":";
			val += util::to_str(config::dmg_joy_b) + ":";
			val += util::to_str(config::dmg_joy_start) + ":";
			val += util::to_str(config::dmg_joy_select) + ":";
			val += util::to_str(config::dmg_joy_left) + ":";
			val += util::to_str(config::dmg_joy_right) + ":";
			val += util::to_str(config::dmg_joy_up) + ":";
			val += util::to_str(config::dmg_joy_down);

			output_lines[line_pos] = "[#dmg_joy_controls:" + val + "]";
		}

		//GBA keyboard controls
		else if(ini_item == "#agb_key_controls")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::agb_key_a) + ":";
			val += util::to_str(config::agb_key_b) + ":";
			val += util::to_str(config::agb_key_start) + ":";
			val += util::to_str(config::agb_key_select) + ":";
			val += util::to_str(config::agb_key_left) + ":";
			val += util::to_str(config::agb_key_right) + ":";
			val += util::to_str(config::agb_key_up) + ":";
			val += util::to_str(config::agb_key_down) + ":";
			val += util::to_str(config::agb_key_l_trigger) + ":";
			val += util::to_str(config::agb_key_r_trigger);

			output_lines[line_pos] = "[#agb_key_controls:" + val + "]";
		}

		//GBA gamepad controls
		else if(ini_item == "#agb_joy_controls")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::agb_joy_a) + ":";
			val += util::to_str(config::agb_joy_b) + ":";
			val += util::to_str(config::agb_joy_start) + ":";
			val += util::to_str(config::agb_joy_select) + ":";
			val += util::to_str(config::agb_joy_left) + ":";
			val += util::to_str(config::agb_joy_right) + ":";
			val += util::to_str(config::agb_joy_up) + ":";
			val += util::to_str(config::agb_joy_down) + ":";
			val += util::to_str(config::agb_joy_l_trigger) + ":";
			val += util::to_str(config::agb_joy_r_trigger);

			output_lines[line_pos] = "[#agb_joy_controls:" + val + "]";
		}

		//Context keyboard controls
		else if(ini_item == "#con_key_controls")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::con_key_left) + ":";
			val += util::to_str(config::con_key_right) + ":";
			val += util::to_str(config::con_key_up) + ":";
			val += util::to_str(config::con_key_down);

			output_lines[line_pos] = "[#con_key_controls:" + val + "]";
		}

		//Context joystick controls
		else if(ini_item == "#con_joy_controls")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::con_joy_left) + ":";
			val += util::to_str(config::con_joy_right) + ":";
			val += util::to_str(config::con_joy_up) + ":";
			val += util::to_str(config::con_joy_down);

			output_lines[line_pos] = "[#con_joy_controls:" + val + "]";
		}

		//Hotkeys
		else if(ini_item == "#hotkeys")
		{
			line_pos = output_count[x];
			std::string val_1 = util::to_str(config::hotkey_turbo);
			std::string val_2 = util::to_str(config::hotkey_mute);
			std::string val_3 = util::to_str(config::hotkey_camera);

			output_lines[line_pos] = "[#hotkeys:" + val_1 + ":" + val_2 + ":" + val_3 + "]";
		}

		//Use CGFX
		else if(ini_item == "#use_cgfx")
		{
			line_pos = output_count[x];
			std::string val = (cgfx::load_cgfx) ? "1" : "0";

			output_lines[line_pos] = "[#use_cgfx:" + val + "]";
		}

		//CGFX manifest path
		else if(ini_item == "#manifest_path")
		{
			line_pos = output_count[x];
			std::string val = (cgfx::manifest_file == "") ? "" : (":'" + cgfx::manifest_file + "'");

			output_lines[line_pos] = "[#manifest_path" + val + "]";
		}

		//CGFX BG Tile dump folder
		else if(ini_item == "#dump_bg_path")
		{
			line_pos = output_count[x];
			std::string val = (cgfx::dump_bg_path == "") ? "" : (":'" + cgfx::dump_bg_path + "'");

			output_lines[line_pos] = "[#dump_bg_path" + val + "]";
		}

		//CGFX OBJ Tile dump folder
		else if(ini_item == "#dump_obj_path")
		{
			line_pos = output_count[x];
			std::string val = (cgfx::dump_obj_path == "") ? "" : (":'" + cgfx::dump_obj_path + "'");

			output_lines[line_pos] = "[#dump_obj_path" + val + "]";
		}

		//CGFX Scaling factor
		else if(ini_item == "#cgfx_scaling_factor")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(cgfx::scaling_factor);

			output_lines[line_pos] = "[#cgfx_scaling_factor:" + val + "]";
		}

		//CGFX Transparency color
		else if(ini_item == "#cgfx_transparency")
		{
			line_pos = output_count[x];
			std::string val = util::to_hex_str(cgfx::transparency_color);

			output_lines[line_pos] = "[#cgfx_transparency:" + val + "]";
		}

		//Use netplay
		else if(ini_item == "#use_netplay")
		{
			line_pos = output_count[x];
			std::string val = (config::use_netplay) ? "1" : "0";

			output_lines[line_pos] = "[#use_netplay:" + val + "]";
		}

		//Use netplay hard sync
		else if(ini_item == "#use_netplay_hard_sync")
		{
			line_pos = output_count[x];
			std::string val = (config::netplay_hard_sync) ? "1" : "0";

			output_lines[line_pos] = "[#use_netplay_hard_sync:" + val + "]";
		}

		//Netplay server port
		else if(ini_item == "#netplay_server_port")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::netplay_server_port);

			output_lines[line_pos] = "[#netplay_server_port:" + val + "]";
		}

		//Netplay client port
		else if(ini_item == "#netplay_client_port")
		{
			line_pos = output_count[x];
			std::string val = util::to_str(config::netplay_client_port);

			output_lines[line_pos] = "[#netplay_client_port:" + val + "]";
		}

		//Netplay client IP address
		else if(ini_item == "#netplay_client_ip")
		{
			line_pos = output_count[x];
			output_lines[line_pos] = "[#netplay_client_ip:" + config::netplay_client_ip + "]";
		}

		else if(ini_item == "#recent_files")
		{
			line_pos = output_count[x];
			output_lines[line_pos] = "[#ignore#]";
		}
	}

	//Write contents to .ini file
	std::ofstream out_file(ini_path.c_str(), std::ios::out);

	if(!out_file.is_open())
	{
		std::cout<<"GBE::Error - Could not save gbe.ini configuration file. Check file path or permissions. \n";
		return false; 
	}

	for(int x = 0; x < output_lines.size(); x++)
	{
		if(output_lines[x] != "[#ignore#]")
		{
			output_lines[x] += "\n";
			out_file << output_lines[x];
		}
	}

	for(int x = 0; x < recent_count; x++)
	{	
		std::string val = "'" + config::recent_files[x] + "'";
		val = "[#recent_files:" + val + "]";
		
		if(x == 0) { out_file << val; }
		else { out_file << "\n" << val; }
	}

	out_file.close();
	return true;
}

/****** Parse the cheats file ******/
bool parse_cheats_file()
{
	if(config::cheats_path.empty()) { return false; }

	std::ifstream file(config::cheats_path.c_str(), std::ios::in); 
	std::string input_line = "";
	std::string line_char = "";

	std::vector<std::string> cheat_entry;
	std::string code_type;
	std::string cheat_code;
	std::string info;

	//Clear cheat codes
	config::gs_cheats.clear();
	config::gg_cheats.clear();
	config::cheats_info.clear();

	if(!file.is_open())
	{
		std::cout<<"GBE::Could not open cheats file " << config::cheats_path << ". Check file path or permissions. \n";
		return false; 
	}

	//Cycle through whole file, line-by-line
	while(getline(file, input_line))
	{
		line_char = input_line[0];
		bool ignore = false;	
		u8 item_count = 0;	

		//Check if line starts with [ - if not, skip line
		if(line_char == "[")
		{
			std::string line_item = "";

			//Cycle through line, character-by-character
			for(int x = 0; ++x < input_line.length();)
			{
				line_char = input_line[x];

				//Check for single-quotes, don't parse ":" or "]" within them
				if((line_char == "'") && (!ignore)) { ignore = true; }
				else if((line_char == "'") && (ignore)) { ignore = false; }

				//Check the character for item limiter : or ] - Push to Vector
				else if(((line_char == ":") || (line_char == "]")) && (!ignore)) 
				{
					cheat_entry.push_back(line_item);
					line_item = "";
					item_count++;
				}

				else { line_item += line_char; }
			}
		}

		if((item_count != 3) && (item_count != 0))
		{
			std::cout<<"GBE::Cheat file has incorrect entry: " << input_line << "\n";
			file.close();
			return false;
		}
	}
	
	file.close();

	//Parse entries
	for(int x = 0; x < cheat_entry.size();)
	{
		code_type = cheat_entry[x++];
		cheat_code = cheat_entry[x++];
		info = cheat_entry[x++];

		//Add Gameshark codes 
		if(code_type == "GS")
		{
			//Verify length
			if(cheat_code.length() == 8)
			{
				//Convert code into u32
				u32 converted_cheat = 0;
				util::from_hex_str(cheat_code, converted_cheat);
				config::gs_cheats.push_back(converted_cheat);
				
				info += "*";
				config::cheats_info.push_back(info);
			}

			else
			{
				std::cout<<"GBE::Error - Could not parse Gameshark cheat code " << cheat_code << "\n";

				config::gs_cheats.clear();
				config::gg_cheats.clear();
				config::cheats_info.clear();

				return false;
			}
		}

		//Add Game Genie codes
		else if(code_type == "GG")
		{
			//Verify length
			if (cheat_code.length() == 9)
			{
				config::gg_cheats.push_back(cheat_code);

				info += "^";
				config::cheats_info.push_back(info);
			}

			else
			{
				std::cout<<"GBE::Error - Could not parse Game Genie cheat code " << cheat_code << "\n";

				config::gs_cheats.clear();
				config::gg_cheats.clear();
				config::cheats_info.clear();

				return false;
			}
		}	
	}

	return true;
}

/****** Saves the cheat file ******/
bool save_cheats_file()
{
	if(config::cheats_path.empty()) { return false; }

	std::ofstream file(config::cheats_path.c_str(), std::ios::out);

	if(!file.is_open())
	{
		std::cout<<"GBE::Could not open cheats file " << config::cheats_path << ". Check file path or permissions. \n";
		return false; 
	}

	int gs_count = 0;
	int gg_count = 0;

	//Cycle through cheats
	for(u32 x = 0; x < config::cheats_info.size(); x++)
	{
		std::string info_str = config::cheats_info[x];
		std::string code_str = "";
		std::string data_str = "";

		//Determine code type based on info string
		std::string last_char = "";
		last_char += info_str[info_str.size() - 1];

		//GS code
		if(last_char == "*")
		{
			info_str.resize(info_str.size() - 1);
			code_str = "GS";
			data_str = util::to_hex_str(config::gs_cheats[gs_count++]).substr(2);

			//Make sure code data length is 8
			while(data_str.size() != 8)
			{
				data_str = "0" + data_str;
			}

			std::string output_line = "[" + code_str + ":" + data_str + ":" + info_str + "]\n";
			file << output_line;
		}

		//GG code
		else if(last_char == "^")
		{
			info_str.resize(info_str.size() - 1);
			code_str = "GG";
			data_str = config::gg_cheats[gg_count++];

			//Make sure code data length is 9
			while(data_str.size() != 9)
			{
				data_str = "0" + data_str;
			}

			std::string output_line = "[" + code_str + ":" + data_str + ":" + info_str + "]\n";
			file << output_line;
		}
	}

	file.close();
	return true;
}
