#pragma once
#include "wx/wx.h"
#include "Settings.h"

class MainWindow;
class DrawingPanel : public wxPanel
{
	//int mGridSize= 15;
	std::vector<std::vector<bool>>& rGameBoard;
	Settings* pSettings = nullptr;
	MainWindow* pParent = nullptr; //use this to refresh
	std::vector<std::vector<int>> mNeighborCount;



public:
	DrawingPanel(MainWindow* _MainWindow, std::vector<std::vector<bool>>& boolVector, std::vector<std::vector<int>> neighborCounts, Settings& _settings); //constructor
	~DrawingPanel(); //destructor

	void OnPaint(wxPaintEvent& _wxPaintEvent);
	void SetSize(const wxSize& size);
	void SetGridSize(int _gridSize);
	
	void OnMouseUp(wxMouseEvent& mouseEvent);
	void SetSettings(Settings settings);
	
	int GetNeighborCount(int a, int b);

	wxDECLARE_EVENT_TABLE();
};

