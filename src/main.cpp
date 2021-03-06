// GB Enhanced+ Copyright Daniel Baxter 2014
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : main.cpp
// Date : August 16, 2014
// Description : The emulator
//
// This is main. It all begins here ;)

#include "gba/core.h"
#include "dmg/core.h"
#include "common/config.h"

int main(int argc, char* args[]) 
{
	std::cout<<"GBE+ 1.1 [SDL]\n";

	core_emu* gbe_plus = NULL;

	//Start SDL from the main thread now, report specific init errors later in the core
	SDL_Init(SDL_INIT_VIDEO);

	//Grab command-line arguments
	for(int x = 0; x++ < argc - 1;) 
	{ 
		std::string temp_arg = args[x]; 
		config::cli_args.push_back(temp_arg);
		parse_filenames();
	}

	//Parse .ini options
	parse_ini_file();

	//Parse cheat file
	if(config::use_cheats) { parse_cheats_file(); }

	if(config::mute) { config::volume = 0; }

	//Parse command-line arguments
	//These will override .ini options!
	if(!parse_cli_args()) { return 0; }

	//Validate the emulated system type
	validate_system_type();

	//Start the appropiate system core - DMG/GBC or GBA
	if(config::gb_type == 3) { gbe_plus = new AGB_core(); }
	else { gbe_plus = new DMG_core(); }
	
	//Read BIOS file optionally
	if(config::use_bios) 
	{
		//If no bios file was passed from the command-line arguments, defer to .ini options
		if(config::bios_file == "")
		{
			switch(config::gb_type)
			{
				case 0x1 : config::bios_file = config::dmg_bios_path; break;
				case 0x2 : config::bios_file = config::gbc_bios_path; break;
				case 0x3 : config::bios_file = config::agb_bios_path; break;
			}
		}

		if(!gbe_plus->read_bios(config::bios_file)) { return 0; } 
	}

	//Read specified ROM file
	if(!gbe_plus->read_file(config::rom_file)) { return 0; }

	//Engage the core
	gbe_plus->start();
	gbe_plus->db_unit.debug_mode = config::use_debugger;

	if(gbe_plus->db_unit.debug_mode) { SDL_CloseAudio(); }

	//Disbale mouse cursor in SDL, it's annoying
	SDL_ShowCursor(SDL_DISABLE);

	//Actually run the core
	gbe_plus->run_core();

	return 0;
}  
