#pragma once
#include <vector>
#include <string>
#include <mutex>
#include "../imgui/imgui.h"
//ints
//doubles/static doubles /Floats 
//bools, etc

//For The UI
//Or Other Stuff


namespace texture //Pictures 
{


	ID3D11ShaderResourceView* ragebot = nullptr;
	ID3D11ShaderResourceView* legitbot = nullptr;
	ID3D11ShaderResourceView* visuals = nullptr;
	ID3D11ShaderResourceView* misc = nullptr;
	ID3D11ShaderResourceView* world = nullptr;
	ID3D11ShaderResourceView* settings = nullptr;
}


namespace UI  // Its Just More Organized 
{
	int tab;   //tab system cs fuck Begintabbar

	bool show = true; //shows The Menu

}

namespace font
{
	inline ImFont* segue_semibold_tabs = nullptr;
	inline ImFont* segue_semibold = nullptr;
	inline ImFont* segue_bold = nullptr;
	inline ImFont* icomoon[15];
}