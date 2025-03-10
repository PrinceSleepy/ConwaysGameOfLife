#pragma once
#include"wx/wx.h"
#include<fstream>


struct Settings
{
	unsigned int livingRed = 128;
	unsigned int livingGreen = 128;
	unsigned int livingBlue = 128;
	unsigned int livingAlpha = 255; 
	unsigned int deadRed = 255; 
	unsigned int deadGreen = 255;
	unsigned int deadBlue = 255;
	unsigned int deadAlpha = 255;

	int gridSize = 15;
	int interval = 50;

	bool displayNeighborCount = false;
	bool isToroidal = false;
	bool showGrid = true;
	bool show10x10Grid = false;


	void SetLivingColor(wxColor color)
	{
		livingRed = color.GetRed();
		livingGreen = color.GetGreen();
		livingBlue = color.GetBlue();
		livingAlpha = color.GetAlpha();
	}

	wxColor GetLivingColor()
	{
		wxColor color(livingRed, livingGreen, livingBlue, livingAlpha);
		return color;
	}

	void SetDeadColor(wxColor color)
	{
		deadRed = color.GetRed();
		deadGreen = color.GetGreen();
		deadBlue = color.GetBlue();
		deadAlpha = color.GetAlpha();
	}

	wxColor GetDeadColor()
	{
		wxColor color(deadRed, deadGreen, deadBlue, deadAlpha);
		return color;
	}

	void LoadFromFile()
	{
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
	}
	void SaveToFile()
	{
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		if (file.is_open())
		{
			file.write((char*)this, sizeof(Settings));
			file.close();
		}
	}
	void ResetSettings()
	{
		livingRed = 128;
		livingGreen = 128;
		livingBlue = 128;
		livingAlpha = 255;
		deadRed = 255;
		deadGreen = 255;
		deadBlue = 255;
		deadAlpha = 255;
		gridSize = 15;
		interval = 50;
	}
	



};
