#include "MainWindow.h"
#include "DrawingPanel.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "time.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnSizeChanged)
	EVT_MENU(10001, MainWindow::PlayEvent)
	EVT_MENU(10003, MainWindow::NextEvent)
	EVT_MENU(10002, MainWindow::PauseEvent)
	EVT_MENU(10004, MainWindow::ClearGrid)
	EVT_TIMER(10005, MainWindow::TimerEvent)
	EVT_MENU(10007, MainWindow::OnDisplayNeighborCount)
	EVT_MENU(10008, MainWindow::OnOpenSettings)
	EVT_MENU(10009, MainWindow::OnRandomize)
	EVT_MENU(10010, MainWindow::OnRandomizeWithSeed)
	EVT_MENU(wxID_NEW, MainWindow::OnNew)
	EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
	EVT_MENU(wxID_SAVE, MainWindow::OnSave)
	EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAs)
	EVT_MENU(wxID_EXIT, MainWindow::OnExit)
	EVT_MENU(10015, MainWindow::OnResetSettings)
	EVT_MENU(10016, MainWindow::OnToggleFinite)
	EVT_MENU(10017, MainWindow::OnToggleToroidal)
	EVT_MENU(10018, MainWindow::OnImport)
	EVT_MENU(10019, MainWindow::OnShowGrid)
	EVT_MENU(10020, MainWindow::OnShow10x10Grid)
wxEND_EVENT_TABLE()


MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0,0), wxSize(500, 500)), mDrawingPanel(new DrawingPanel(this, mGameBoard, mNeighborCounts, settings))
{
	settings.LoadFromFile();
	settings.showGrid = true;
	mStatusBar = CreateStatusBar();
	UpdateStatusBar();
	mToolBar = CreateToolBar();
	wxBitmap playIcon(play_xpm);
	mToolBar->AddTool(10001, "Play", playIcon);
	wxBitmap pauseIcon(pause_xpm);
	mToolBar->AddTool(10002, "Pause", pauseIcon);
	wxBitmap nextIcon(next_xpm);
	mToolBar->AddTool(10003, "Next", nextIcon);
	wxBitmap trashIcon(trash_xpm);
	mToolBar->AddTool(10004, "Clear Grid", trashIcon);
	mToolBar->Realize();
	mTimer = new wxTimer(this, 10005);
	_sizer = new wxBoxSizer(wxVERTICAL);
	_sizer->Add(mDrawingPanel, 1, wxEXPAND | wxALL);
	SetSizer(_sizer);
	InitializeGrid();
	this->Layout();

	mMenuBar = new wxMenuBar();

	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW);
	fileMenu->Append(wxID_OPEN);
	fileMenu->Append(wxID_SAVE);
	fileMenu->Append(wxID_SAVEAS);
	fileMenu->Append(10018, "Import");
	fileMenu->Append(wxID_EXIT);
	mMenuBar->Append(fileMenu, "File");

	wxMenu* viewMenu = new wxMenu();
	wxMenuItem* displayNeighborCountMenuItem = new wxMenuItem(viewMenu, 10007, "Display Neighbor Count", "", wxITEM_CHECK);
	displayNeighborCountMenuItem->SetCheckable(true);
	viewMenu->Append(displayNeighborCountMenuItem);
	mFiniteMenuItem = new wxMenuItem(viewMenu, 10016, "Finite Universe", "", wxITEM_CHECK);
	mFiniteMenuItem->SetCheckable(true);
	viewMenu->Append(mFiniteMenuItem);
	mToroidalMenuItem = new wxMenuItem(viewMenu, 10017, "Toroidal Universe", "", wxITEM_CHECK);
	mToroidalMenuItem->SetCheckable(true);
	viewMenu->Append(mToroidalMenuItem);
	wxMenuItem* showGridMenuItem = new wxMenuItem(viewMenu, 10019, "Show Grid", "", wxITEM_CHECK);
	showGridMenuItem->SetCheckable(true);
	viewMenu->Append(showGridMenuItem);
	showGridMenuItem->Check(settings.showGrid);
	wxMenuItem* show10x10GridMenuItem = new wxMenuItem(viewMenu, 10020, "Show 10x10 Grid", "", wxITEM_CHECK);
	show10x10GridMenuItem->SetCheckable(true);
	viewMenu->Append(show10x10GridMenuItem);
	show10x10GridMenuItem->Check(settings.show10x10Grid);
	mMenuBar->Append(viewMenu, "View");

	wxMenu* optionsMenu = new wxMenu();
	optionsMenu->Append(10008, "Settings");
	optionsMenu->Append(10015, "Reset Settings");
	optionsMenu->Append(10009, "Randomize");
	optionsMenu->Append(10010, "Randomize with Seed");
	mMenuBar->Append(optionsMenu, "Options");


	SetMenuBar(mMenuBar);
	mFiniteMenuItem->Check(true);
	settings.isToroidal = false;

};

MainWindow::~MainWindow()
{
	
}

void MainWindow::OnSizeChanged(wxSizeEvent& event)
{
	if (mDrawingPanel != nullptr)
	{
		mDrawingPanel->SetSize(event.GetSize());
		mDrawingPanel->Refresh();
		event.Skip();
	}
}

void MainWindow::InitializeGrid()
{
	
	mGameBoard.resize(settings.gridSize);
	for (int i = 0; i < settings.gridSize; i++)
	{
		mGameBoard[i].resize(settings.gridSize);
	}
	mDrawingPanel->SetGridSize(settings.gridSize);
}

void MainWindow::UpdateStatusBar()
{
	wxString statusBarString = wxString::Format("Generations: %d    Living Cells: %d", mGenerationsNum, mLivingCells);
	mStatusBar->SetStatusText(statusBarString, 0);
}

void MainWindow::PlayEvent(wxCommandEvent& event)
{
	mTimer->Start(settings.interval);
}

void MainWindow::PauseEvent(wxCommandEvent& event)
{
	mTimer->Stop();
}

void MainWindow::NextEvent(wxCommandEvent& event)
{
	NextGeneration();
}

void MainWindow::ClearGrid(wxCommandEvent& event)
{
	for (int i = 0; i < settings.gridSize; i++)
	{
		for (int j = 0; j < settings.gridSize; j++)
		{
			mGameBoard[i][j] = false;
		}
	}
	mGenerationsNum = 0;
	mLivingCells = 0;
	UpdateStatusBar();
	mDrawingPanel->Refresh();
}

int MainWindow::NeighborCount(int a, int b)
{
	int count = 0;
	int gridSize = settings.gridSize;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (i == 0 && j == 0)
			{
				continue; 
			}

			int row = a + i;
			int col = b + j;

			if (settings.isToroidal) 
			{
				
				if (row < 0) {
					row = gridSize - 1;
				}
				else if (row >= gridSize) {
					row = 0;
				}

				if (col < 0) {
					col = gridSize - 1;
				}
				else if (col >= gridSize) {
					col = 0;
				}
			}
			else 
			{
			
				if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
					continue; 
				}
			}

			if (mGameBoard[row][col])
			{
				count++;
			}
		}
	}
	return count;
}

void MainWindow::NextGeneration()
{
	mLivingCells = 0;
	mGenerationsNum++;
	std::vector<std::vector<bool>> sandbox;
	sandbox.resize(settings.gridSize);
	for(int i = 0; i < settings.gridSize; i++)
	{
		sandbox[i].resize(settings.gridSize);
	}
	for (int i = 0; i < settings.gridSize; i++)
	{
		for (int j = 0; j < settings.gridSize; j++)
		{
			int livingNeighbors = NeighborCount(i, j);
			bool isAlive = mGameBoard[i][j];
			if (isAlive)
			{
				if (livingNeighbors == 2 || livingNeighbors == 3)
				{
					sandbox[i][j] = true;
					mLivingCells++;
				}
				else
				{
					sandbox[i][j] = false;
				}
			}
			else
			{
				if (livingNeighbors == 3)
				{
					sandbox[i][j] = true;
					mLivingCells++;
				}
				else
				{
					sandbox[i][j] = false;
				}
			}
		}
	}
	mGameBoard.swap(sandbox);
	UpdateStatusBar();
	mDrawingPanel->Refresh();
	
}

void MainWindow::TimerEvent(wxTimerEvent& event)
{
	NextGeneration();
}

Settings MainWindow::GetSettings()
{
	return settings;
}

void MainWindow::OnOpenSettings(wxCommandEvent& event)
{
	SettingsDialog settingsDialog(*this, settings);
	settingsDialog.ShowModal();
	if (settingsDialog.ShowModal()== wxID_OK)
	{
		settings.SaveToFile();
		InitializeGrid();
		mDrawingPanel->Refresh();
	}
}
void MainWindow::RandomizeGrid(int seed)
{
	srand(seed);
	for (int i = 0; i < settings.gridSize; i++)
	{
		for (int j = 0; j < settings.gridSize; j++)
		{
			bool isAlive = rand() % 2 == 0;
			mGameBoard[i][j] = isAlive;
		}
	}
	mDrawingPanel->Refresh();
}

void MainWindow::OnRandomize(wxCommandEvent& event)
{
	RandomizeGrid(time(NULL));
}

void MainWindow::OnRandomizeWithSeed(wxCommandEvent& event)
{
	long seed = wxGetNumberFromUser("Enter Seed", "Seed", "Randomize with Seed", 0, 0, LONG_MAX, this);
	{
		RandomizeGrid(seed);
	}
}

void MainWindow::OnNew(wxCommandEvent& event)
{
	mGameBoard.clear();
	InitializeGrid();
	mDrawingPanel->Refresh();
}

void MainWindow::OnOpen(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Open Game of Life File", "", "", "Game of Life Files(*.cells | *.cells", wxFD_OPEN);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	mGameBoard.clear();
	std::string buffer;
	std::ifstream fileStream;
	fileStream.open((std::string)fileDialog.GetPath());
	if (fileStream.is_open())
	{
		while(std::getline(fileStream, buffer))
		{
			std::vector<bool> column;
			for (char cell : buffer)
			{
				if (cell == '*')
				{
					column.push_back(true);
				}
				else
				{
					column.push_back(false);
				}
			}
			mGameBoard.push_back(column);
		
		}
		fileStream.close();
		settings.gridSize = mGameBoard.size();
		mDrawingPanel->Refresh();
	}
	
	event.Skip();
}

void MainWindow::OnSave(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Save Game of Life File", "", "", "Game of Life Files(*.cells | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	std::ofstream fileStream;
	fileStream.open((std::string)fileDialog.GetPath());
	if (fileStream.is_open())
	{
		for (int i = 0; i < settings.gridSize; i++)
		{
			for (int j = 0; j < settings.gridSize; j++)
			{
				if (mGameBoard[i][j])
				{
					fileStream << '*';
				}
				else
				{
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}
		fileStream.close();
	}
	event.Skip();
}

void MainWindow::OnSaveAs(wxCommandEvent& event)
{
	OnSave(event);
	event.Skip();
}

void MainWindow::OnExit(wxCommandEvent& event)
{
	Close();
}

void MainWindow::OnResetSettings(wxCommandEvent& event)
{
	settings.ResetSettings();
	settings.SaveToFile();
	InitializeGrid();
	mDrawingPanel->Refresh();
}

void MainWindow::OnToggleFinite(wxCommandEvent& event)
{
	mToroidalMenuItem->Check(false);
	settings.isToroidal = false;
	event.Skip();
}

void MainWindow::OnToggleToroidal(wxCommandEvent& event)
{
	mFiniteMenuItem->Check(false);
	settings.isToroidal = true;
	event.Skip();
}

void MainWindow::OnImport(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Open Game of Life File", "", "", "Game of Life Files(*.cells | *.cells", wxFD_OPEN);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	std::vector<std::vector<bool>> importedGameBoard;
	std::string buffer;
	std::ifstream fileStream;
	fileStream.open((std::string)fileDialog.GetPath());
	if (fileStream.is_open()) {
		while (std::getline(fileStream, buffer)) {
			std::vector<bool> column;
			for (char cell : buffer) {
				if (cell == '*') {
					column.push_back(true);
				}
				else {
					column.push_back(false);
				}
			}
			importedGameBoard.push_back(column);
		}
		fileStream.close();
		int gridSize = mGameBoard.size();
		int importedHeight = importedGameBoard.size();
		int importedWidth = (importedHeight > 0) ? importedGameBoard[0].size() : 0;
		int startX = (gridSize - importedHeight) / 2;
		int startY = (gridSize - importedWidth) / 2;
		for (int i = 0; i < importedHeight; ++i) {
			for (int j = 0; j < importedWidth; ++j) {
				if (startX + i >= 0 && startX + i < gridSize && startY + j >= 0 && startY + j < gridSize) {
					mGameBoard[startX + i][startY + j] = importedGameBoard[i][j];
				}
			}
		}
		mDrawingPanel->Refresh();
	}
}

void MainWindow::OnShowGrid(wxCommandEvent& event)
{
	settings.showGrid = !settings.showGrid;
	mDrawingPanel->Refresh();
	event.Skip();
}

void MainWindow::OnShow10x10Grid(wxCommandEvent& event)
{
	settings.show10x10Grid = !settings.show10x10Grid;
	mDrawingPanel->Refresh();
	event.Skip();
}


void MainWindow::OnDisplayNeighborCount(wxCommandEvent& event)
{
	settings.displayNeighborCount = !settings.displayNeighborCount;
	mDrawingPanel->Refresh();
}




