
#include <iostream>
#include <chrono>
#include <bitset>
#include <fstream>
#include <sstream>

//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "spdlog/spdlog.h"

#include "platform.h"

#include "magma/Console.h"
#include "magma/input.h"
#include "generate/generate.h"
#include "generate/enums.h"

using magma::Console;
using magma::CommandManager;
using namespace magma;

void init_logging()
{
	try
	{
		std::vector<spdlog::sink_ptr> sinks;
#if defined(Windows)
		sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
#else
		sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
#endif
		sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("rlv3.log"));
		auto console_logger = std::make_shared<spdlog::logger>("console", sinks[0]);
		auto main_logger = std::make_shared<spdlog::logger>("main", begin(sinks), end(sinks));
		spdlog::register_logger(console_logger);
		spdlog::register_logger(main_logger);
		console_logger->set_level(spdlog::level::info);
		main_logger->set_level(spdlog::level::info);
		console_logger->info("Testing console logger!");
		main_logger->info("Testing combined logger!");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log initialization failed: " + std::string(ex.what()) + "\n";
		exit(EXIT_FAILURE);
	}
}

bool stoint(int &i, char const *s, int base = 0);


int main(int argc, char* argk[])
{
	init_logging();
	auto log = spdlog::get("main");
	log->info("Creating Screen.");
	magma::init();
	auto root = Console::init_root(100, 100, "Taffer_10x10.png", 10);
	auto con = Console::init(40, 20);

	int num = SDL_GetNumRenderDrivers();
	SDL_RendererInfo info;
	for (int i = 0; i < num; i++)
	{
		SDL_GetRenderDriverInfo(i, &info);
		log->info("Driver: {} is {}", i, info.name);
	}
	CommandManager rlCommands = CommandManager();
	//					  INPUT DOMAIN				                     COMMAND	    	NAME(uniq)	IS KB	KEYCODE			PRESSED,SHIFT,CTRL,ALT,CAPS	
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_E,		"move_e",	true ,	SDLK_RIGHT,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_N,		"move_n",	true ,	SDLK_UP,		true ,false,false,false,false));	
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_NE,		"move_ne",	true ,	SDLK_RIGHT,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_W,		"move_w",	true ,	SDLK_LEFT,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_NW,		"move_nw",	true ,	SDLK_LEFT,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_S,		"move_s",	true ,	SDLK_DOWN,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_SE,		"move_se",	true ,	SDLK_RIGHT,		true ,false,true ,false,false));	
	rlCommands.addCommand(magma::INPUT_DOMAIN_DEFAULT, magma::CommandDef(CMD_MOVE_SW,		"move_sw",	true ,	SDLK_LEFT,		true ,false,true ,false,false));

	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_SPACE_K,		"txt_spc",	true ,	SDLK_SPACE,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_EXCL_K,		"txt_!",	true ,	SDLK_1,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_QUOTE_K,		"txt_\"",	true ,	SDLK_2,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_HASH_K,		"txt_#",	true ,	SDLK_3,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_DOLLAR_K,		"txt_$",	true ,	SDLK_4,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_PERCENT_K,		"txt_%",	true ,	SDLK_5,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_AMPERSAND_K,	"txt_&",	true ,	SDLK_7,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_APOSTROPHE_K,	"txt_'",	true ,	SDLK_QUOTE,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_LEFT_PAREN_K,	"txt_(",	true ,	SDLK_9,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_RIGHT_PAREN_K,	"txt_)",	true ,	SDLK_0,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_ASTERIX_K,		"txt_*",	true ,	SDLK_8,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_PLUS_K,		"txt_+",	true ,	SDLK_EQUALS,	true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_COMMA_K,		"txt_,",	true ,	SDLK_COMMA,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_MINUS_K,		"txt_-",	true ,	SDLK_MINUS,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_STOP_K,		"txt_.",	true ,	SDLK_PERIOD,	true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_SLASH_K,		"txt_/",	true ,	SDLK_SLASH,		true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_0_K,			"txt_0",	true ,	SDLK_0,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_1_K,			"txt_1",	true ,	SDLK_1,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_2_K,			"txt_2",	true ,	SDLK_2,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_3_K,			"txt_3",	true ,	SDLK_3,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_4_K,			"txt_4",	true ,	SDLK_4,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_5_K,			"txt_5",	true ,	SDLK_5,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_6_K,			"txt_6",	true ,	SDLK_6,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_7_K,			"txt_7",	true ,	SDLK_7,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_8_K,			"txt_8",	true ,	SDLK_8,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_9_K,			"txt_9",	true ,	SDLK_9,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_COLON_K,		"txt_:",	true ,	SDLK_SEMICOLON,	true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_SEMICOLON_K,	"txt_;",	true ,	SDLK_SEMICOLON, true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_LT_K,			"txt_<",	true ,	SDLK_COMMA,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_EQUALS_K,		"txt_=",	true ,	SDLK_EQUALS,	true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_GT_K,			"txt_>",	true ,	SDLK_PERIOD,	true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_QUESTION_K,	"txt_?",	true ,	SDLK_SLASH,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_AT_K,			"txt_@",	true ,	SDLK_QUOTE,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_A_K,			"txt_A",	true ,	SDLK_a,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_A_K,			"txt_A_cap",true ,	SDLK_a,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_a_K,			"txt_a",	true ,	SDLK_a,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_B_K,			"txt_B",	true ,	SDLK_b,			true ,true ,false,false,false));			
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_B_K,			"txt_B_cap",true ,	SDLK_b,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_b_K,			"txt_b",	true ,	SDLK_b,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_C_K,			"txt_C",	true ,	SDLK_c,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_C_K,			"txt_C_cap",true ,	SDLK_c,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_c_K,			"txt_c",	true ,	SDLK_c,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_D_K,			"txt_D",	true ,	SDLK_d,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_D_K,			"txt_D_cap",true ,	SDLK_d,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_d_K,			"txt_d",	true ,	SDLK_d,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_E_K,			"txt_E",	true ,	SDLK_e,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_E_K,			"txt_E_cap",true ,	SDLK_e,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_e_K,			"txt_e",	true ,	SDLK_e,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_F_K,			"txt_F",	true ,	SDLK_f,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_F_K,			"txt_F_cap",true ,	SDLK_f,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_f_K,			"txt_f",	true ,	SDLK_f,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_G_K,			"txt_G",	true ,	SDLK_g,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_G_K,			"txt_G_cap",true ,	SDLK_g,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_g_K,			"txt_g",	true ,	SDLK_g,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_H_K,			"txt_H",	true ,	SDLK_h,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_H_K,			"txt_H_cap",true ,	SDLK_h,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_h_K,			"txt_h",	true ,	SDLK_h,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_I_K,			"txt_I",	true ,	SDLK_i,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_I_K,			"txt_I_cap",true ,	SDLK_i,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_i_K,			"txt_i",	true ,	SDLK_i,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_J_K,			"txt_J",	true ,	SDLK_j,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_J_K,			"txt_J_cap",true ,	SDLK_j,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_j_K,			"txt_j",	true ,	SDLK_j,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_K_K,			"txt_K",	true ,	SDLK_k,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_K_K,			"txt_K_cap",true ,	SDLK_k,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_k_K,			"txt_k",	true ,	SDLK_k,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_L_K,			"txt_L",	true ,	SDLK_l,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_L_K,			"txt_L_cap",true ,	SDLK_l,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_l_K,			"txt_l",	true ,	SDLK_l,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_M_K,			"txt_M",	true ,	SDLK_m,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_M_K,			"txt_M_cap",true ,	SDLK_m,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_m_K,			"txt_m",	true ,	SDLK_m,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_N_K,			"txt_N",	true ,	SDLK_n,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_N_K,			"txt_N_cap",true ,	SDLK_n,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_n_K,			"txt_n",	true ,	SDLK_n,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_O_K,			"txt_O",	true ,	SDLK_o,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_O_K,			"txt_O_cap",true ,	SDLK_o,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_o_K,			"txt_o",	true ,	SDLK_o,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_P_K,			"txt_P",	true ,	SDLK_p,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_P_K,			"txt_P_cap",true ,	SDLK_p,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_p_K,			"txt_p",	true ,	SDLK_p,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_Q_K,			"txt_Q",	true ,	SDLK_q,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_Q_K,			"txt_Q_cap",true ,	SDLK_q,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_q_K,			"txt_q",	true ,	SDLK_q,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_R_K,			"txt_R",	true ,	SDLK_r,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_R_K,			"txt_R_cap",true ,	SDLK_r,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_r_K,			"txt_r",	true ,	SDLK_r,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_S_K,			"txt_S",	true ,	SDLK_s,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_S_K,			"txt_S_cap",true ,	SDLK_s,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_s_K,			"txt_s",	true ,	SDLK_s,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_T_K,			"txt_T",	true ,	SDLK_t,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_T_K,			"txt_T_cap",true ,	SDLK_t,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_t_K,			"txt_t",	true ,	SDLK_t,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_U_K,			"txt_U",	true ,	SDLK_u,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_U_K,			"txt_U_cap",true ,	SDLK_u,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_u_K,			"txt_u",	true ,	SDLK_u,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_V_K,			"txt_V",	true ,	SDLK_v,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_V_K,			"txt_V_cap",true ,	SDLK_v,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_v_K,			"txt_v",	true ,	SDLK_v,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_W_K,			"txt_W",	true ,	SDLK_w,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_W_K,			"txt_W_cap",true ,	SDLK_w,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_w_K,			"txt_w",	true ,	SDLK_w,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_X_K,			"txt_X",	true ,	SDLK_x,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_X_K,			"txt_X_cap",true ,	SDLK_x,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_x_K,			"txt_x",	true ,	SDLK_x,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_Y_K,			"txt_Y",	true ,	SDLK_y,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_Y_K,			"txt_Y_cap",true ,	SDLK_y,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_y_K,			"txt_y",	true ,	SDLK_y,			true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_Z_K,			"txt_Z",	true ,	SDLK_z,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_Z_K,			"txt_Z_cap",true ,	SDLK_z,			true ,false,false,false,true ));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_z_K,			"txt_z",	true ,	SDLK_z,			true ,false,false,false,false));
	// IM NOT REFORMATTING AGAIN.
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_L_SQ_K,		"txt_[",	true ,	SDLK_LEFTBRACKET,	true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_B_SLASH_K,		"txt_\\",	true ,	SDLK_BACKSLASH, true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_R_SQ_K,		"txt_]",	true ,  SDLK_RIGHTBRACKET,	true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_CARET_K,		"txt_^",	true ,	SDLK_6,			true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_UNDERSCORE_K,	"txt__",	true ,	SDLK_MINUS,		true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_GRAVE_K,		"txt_`",	true ,	SDLK_BACKQUOTE,	true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_L_BRACKET_K,	"txt_{",	true ,	SDLK_LEFTBRACKET,	true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_PIPE_K,		"txt_|",	true ,	SDLK_BACKSLASH,	true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_R_BRACKET_K,	"txt_}",	true ,	SDLK_RIGHTBRACKET,	true ,true ,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_TILDA_K,		"txt_~",	true ,	SDLK_BACKQUOTE,	true ,true ,false,false,false));

	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_BACKSPACE,		"txt_BK",	true ,	SDLK_BACKSPACE,	true ,false,false,false,false));
	rlCommands.addCommand(magma::INPUT_DOMAIN_TEXTBOX, magma::CommandDef(CMD_ENTER,			"txt_RT",	true ,	SDLK_RETURN,	true ,true ,false,false,false));


	log->info("{}", sizeof(magma::inputDomains));










	const Uint8* state = SDL_GetKeyboardState(NULL);
	int modX = 0;
	int modY = 0;
	auto map = Map(256, 256, 1234);
	map.generate_heightmap();
	map.generate_moisturemap();
	map.assign_biomes();
	int rWidth = 80;
	int rHeight = 40;
	rlCommands.activateDomain(magma::INPUT_DOMAIN_DEFAULT);
	rlCommands.activateDomain(magma::INPUT_DOMAIN_TEXTBOX);
	while (true)
	{
		for (int x = 0; x < rWidth; x++)
		{
			for (int y = 0; y < rHeight; y++)
			{
				Biome b;
				if (modX + x < 0 || modY + y < 0 || modX + x > 255 || modY + y > 255)
					b = Biome::OCEAN;
				else 
					b = map.biomemap[modX + x][modY + y];
				switch(b)
				{
				case Biome::OCEAN:
					root->set_fg(70, 17, 219);
					root->putc(x, y, 'O');
					break;
				case Biome::BEACH:
					root->set_fg(229, 244, 66);
					root->putc(x, y, 'B');
					break;
				case Biome::GRASSLAND:
					root->set_fg(17, 219, 27);
					root->putc(x, y, 'G');
					break;
				case Biome::DESERT:
					root->set_fg(229, 244, 66);
					root->putc(x, y, 'D');
					break;
				case Biome::MOUNTAINS:
					root->set_fg(255, 255, 255);
					root->putc(x, y, 'M');
					break;
				}
				
			}
		}
		
		magma::commands cmd;
		std::string name;
		std::string txt;
		while (rlCommands.getCommand(cmd, name))
		{
			log->info("Command: {}, {}", (char)cmd, name);
		}


		root->refresh();
		root->clear();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				rlCommands.checkCommand(event.type, event);
				break;
			case SDL_QUIT:
				exit(0);
			}
		}

	}
	return 0;
}
