#pragma once
#include "wx/wx.h"
#include "Settings.h"
#include "SettingsDialog.h"



class DrawingPanel;
class MainWindow : public wxFrame
{
    DrawingPanel* mDrawingPanel = nullptr;
    wxBoxSizer* _sizer = nullptr;
    std::vector<std::vector<bool>> mGameBoard;
    int mGenerationsNum;
    int mLivingCells;
    wxStatusBar* mStatusBar = nullptr;
    wxToolBar* mToolBar = nullptr;
    wxTimer* mTimer = nullptr;
    //int fireTime = 50; //50 milliseconds
    Settings settings;
    wxMenuBar* mMenuBar = nullptr;
    std::vector<std::vector<int>> mNeighborCounts;
    wxMenuItem* mFiniteMenuItem = nullptr;
    wxMenuItem* mToroidalMenuItem = nullptr;
    
public:
    MainWindow(); //default constructor
    ~MainWindow(); //destructor

    void OnSizeChanged(wxSizeEvent& event);

    void InitializeGrid();

    void UpdateStatusBar();

    void PlayEvent(wxCommandEvent& event);
    void PauseEvent(wxCommandEvent& event);
    void NextEvent(wxCommandEvent& event);
    void ClearGrid(wxCommandEvent& event);

    int NeighborCount(int a, int b);
    void NextGeneration();

    void TimerEvent(wxTimerEvent& event);
    Settings GetSettings();

    void OnOpenSettings(wxCommandEvent& event);
    void OnDisplayNeighborCount(wxCommandEvent& event);
    void RandomizeGrid(int seed); 
    void OnRandomize(wxCommandEvent& event);
    void OnRandomizeWithSeed(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnResetSettings(wxCommandEvent& event);
    void OnToggleFinite(wxCommandEvent& event);
    void OnToggleToroidal(wxCommandEvent& event);
    void OnImport(wxCommandEvent& event);
    void OnShowGrid(wxCommandEvent& event);
    void OnShow10x10Grid(wxCommandEvent& event);
   

  wxDECLARE_EVENT_TABLE();
};

