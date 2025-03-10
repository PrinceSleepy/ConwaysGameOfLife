#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
	EVT_PAINT(DrawingPanel::OnPaint)
	EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()


DrawingPanel::DrawingPanel(MainWindow* _MainWindow, std::vector<std::vector<bool>>& boolVector, std::vector<std::vector<int>> neighborCounts, Settings& _settings) : wxPanel(_MainWindow, wxID_ANY, wxPoint(0, 0), wxSize(200,200)), rGameBoard(boolVector), mNeighborCount(neighborCounts), pSettings(&_settings)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

DrawingPanel::~DrawingPanel()
{
}



void DrawingPanel::OnPaint(wxPaintEvent& _wxPaintEvent)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    wxGraphicsContext* graphicsContext = wxGraphicsContext::Create(dc);
    if (!graphicsContext)
    {
        return;
    }
    graphicsContext->SetBrush(*wxWHITE);

    float cellWidth = this->GetSize().x / (float)pSettings->gridSize;
    float cellHeight = this->GetSize().y / (float)pSettings->gridSize;

    // Check if grid should be shown
    if (pSettings->showGrid) 
    {
        graphicsContext->SetPen(*wxBLACK);
    }
    else 
    {
        graphicsContext->SetPen(wxPen(wxTransparentColor));
    }
    if (pSettings->show10x10Grid) 
    {
        {
            int lines = pSettings->gridSize / 10+1;
            wxPen pen(*wxBLACK, 15); 
            dc.SetPen(pen);
            for (int i = 1; i < lines; i++)
            {
                dc.DrawLine(wxPoint(cellWidth * 10 * i, 0), wxPoint(cellWidth * 10 * i, this->GetSize().y));
            }

            for (int i = 1; i < lines; i++)
            {
                dc.DrawLine(wxPoint(0, cellHeight * 10 * i), wxPoint(this->GetSize().x, cellHeight * 10 * i));
            }
        }
    }
    for (int i = 0; i < pSettings->gridSize; i++)
    {
        for (int j = 0; j < pSettings->gridSize; j++)
        {
            if (rGameBoard[i][j] == false)
            {
                graphicsContext->SetBrush(pSettings->GetDeadColor()); //need to set color to deadcellcolor
            }
            else
            {
                graphicsContext->SetBrush(pSettings->GetLivingColor()); //need to set color to livingcellcolor
            }
            graphicsContext->DrawRectangle(cellWidth * i, cellHeight * j, cellWidth, cellHeight);

            //display neighborCount functionality
            if (pSettings->displayNeighborCount)
            {
                wxString neighborCountString = wxString::Format("%d", GetNeighborCount(i, j));
                wxFontInfo fontInfo(16);
                wxFont font(fontInfo);
                graphicsContext->SetFont(font, *wxBLACK);

                wxDouble textWidth;
                wxDouble textHeight;
                graphicsContext->GetTextExtent(neighborCountString, &textWidth, &textHeight);
                float textX = cellWidth * i + (cellWidth - textWidth) / 2.0f;
                float textY = cellHeight * j + (cellHeight - textHeight) / 2.0f;
                graphicsContext->DrawText(neighborCountString, textX, textY);
            }
        }
    }

    delete graphicsContext;
}


void DrawingPanel::SetSize(const wxSize& size)
{
	wxPanel::SetSize(size);
	Refresh();
}

void DrawingPanel::SetGridSize(int _gridSize)
{
	pSettings->gridSize = _gridSize;
	Refresh();

}

void DrawingPanel::OnMouseUp(wxMouseEvent& mouseEvent)
{
	int xCoordinate = mouseEvent.GetX();
	int yCoordinate = mouseEvent.GetY();
	int cellWidth = this->GetSize().x / pSettings->gridSize;
	int cellHeight = this->GetSize().y / pSettings->gridSize;
	int i = xCoordinate / cellWidth;
	int j = yCoordinate / cellHeight;

	if (i >= 0 && i < pSettings->gridSize && j >= 0 && j < pSettings->gridSize)
	{
		rGameBoard[i][j] = !rGameBoard[i][j];
	}
	Refresh();
}

int DrawingPanel::GetNeighborCount(int a, int b)
{
	int count = 0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (a + i < 0 || b + j < 0)
			{
				continue;
			}
			if (a + i >= pSettings->gridSize || b + j >= pSettings->gridSize)
			{
				continue;
			}
			if (i == 0 && j == 0)
			{
				continue;
			}
			bool isAlive = rGameBoard[a + i][b + j];
			if (isAlive)
			{
				count++;
			}
		}
	}
	return count;

}

