#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Container/Stack.h"

int  m_nMazeWidth;
int  m_nMazeHeight;
int *m_maze;

#define CELL_PATH_N  	0x01
#define CELL_PATH_E  	0x02
#define CELL_PATH_S  	0x04
#define CELL_PATH_W  	0x08
#define CELL_VISITED  	0x10

int  m_nVisitedCells;	
Stack m_stack;
int  m_nPathWidth;
int Size = 10;

int Offset(int x,int y){
	return (((Vic2*)Stack_Top(&m_stack))->y + y) * m_nMazeWidth + (((Vic2*)Stack_Top(&m_stack))->x + x);
}

void Setup(AlxWindow* w){
	m_stack = Stack_Make(sizeof(Vic2),GetWidth() * GetHeight());

	m_nMazeWidth = 25;
	m_nMazeHeight = 25;		
	m_maze = (int*)calloc(m_nMazeWidth * m_nMazeHeight,sizeof(int));
	memset(m_maze,0x00,m_nMazeWidth * m_nMazeHeight * sizeof(int));
	m_nPathWidth = 3;
	
	int x = Random_i32_MinMax(0,m_nMazeWidth);
	int y = Random_i32_MinMax(0,m_nMazeHeight);
	Stack_Push(&m_stack,(Vic2[]){ { x,y } });
	m_maze[y * m_nMazeWidth + x] = CELL_VISITED;
	m_nVisitedCells = 1;
}

void Update(AlxWindow* w){
    Thread_Sleep_M(10);
	
	if (m_nVisitedCells < m_nMazeWidth * m_nMazeHeight)
	{
		Vector neighbours = Vector_New(sizeof(int));
		if (((Vic2*)Stack_Top(&m_stack))->y > 0 && (m_maze[Offset(0, -1)] & CELL_VISITED) == 0)
			Vector_Push(&neighbours,(int[]){ 0 });
		
		if (((Vic2*)Stack_Top(&m_stack))->x < m_nMazeWidth - 1 && (m_maze[Offset(1, 0)] & CELL_VISITED) == 0)
			Vector_Push(&neighbours,(int[]){ 1 });
		
		if (((Vic2*)Stack_Top(&m_stack))->y < m_nMazeHeight - 1 && (m_maze[Offset(0, 1)] & CELL_VISITED) == 0)
			Vector_Push(&neighbours,(int[]){ 2 });
		
		if (((Vic2*)Stack_Top(&m_stack))->x > 0 && (m_maze[Offset(-1, 0)] & CELL_VISITED) == 0)
			Vector_Push(&neighbours,(int[]){ 3 });
		
		if (!neighbours.size==0){
			int next_cell_dir = *((int*)Vector_Get(&neighbours,Random_i32_MinMax(0,neighbours.size)));
			
			switch (next_cell_dir)
			{
			case 0: // North
				m_maze[Offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
				m_maze[Offset(0,  0)] |= CELL_PATH_N;
				Stack_Push(&m_stack,(Vic2[]){ { ((Vic2*)Stack_Top(&m_stack))->x,((Vic2*)Stack_Top(&m_stack))->y-1 } });
				break;
			case 1: // East
				m_maze[Offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
				m_maze[Offset( 0, 0)] |= CELL_PATH_E;
				Stack_Push(&m_stack,(Vic2[]){ { ((Vic2*)Stack_Top(&m_stack))->x+1,((Vic2*)Stack_Top(&m_stack))->y } });
				break;
			case 2: // South
				m_maze[Offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
				m_maze[Offset(0,  0)] |= CELL_PATH_S;
				Stack_Push(&m_stack,(Vic2[]){ { ((Vic2*)Stack_Top(&m_stack))->x,((Vic2*)Stack_Top(&m_stack))->y+1 } });
				break;
			case 3: // West
				m_maze[Offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
				m_maze[Offset( 0, 0)] |= CELL_PATH_W;
				Stack_Push(&m_stack,(Vic2[]){ { ((Vic2*)Stack_Top(&m_stack))->x-1,((Vic2*)Stack_Top(&m_stack))->y } });
				break;
			}
			m_nVisitedCells++;
		}else{
			Stack_PopTop(&m_stack);
		}
	}
	
	Clear(DARK_BLUE);
	
	for (int x = 0; x < m_nMazeWidth; x++)
	{
		for (int y = 0; y < m_nMazeHeight; y++)
		{
			for (int py = 0; py < m_nPathWidth; py++)
				for (int px = 0; px < m_nPathWidth; px++){
					if (m_maze[y * m_nMazeWidth + x] & CELL_VISITED)
						RenderRect((x * (m_nPathWidth + 1) + px) * Size,(y * (m_nPathWidth + 1) + py) * Size,Size,Size,WHITE); // Draw Cell
					else
						RenderRect((x * (m_nPathWidth + 1) + px) * Size,(y * (m_nPathWidth + 1) + py) * Size,Size,Size,BLUE); // Draw Cell
				}
			
			for (int p = 0; p < m_nPathWidth; p++){
				if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_S)
					RenderRect((x * (m_nPathWidth + 1) + p) * Size,(y * (m_nPathWidth + 1) + m_nPathWidth) * Size,Size,Size,WHITE); // Draw South Passage
				if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_E)
					RenderRect((x * (m_nPathWidth + 1) + m_nPathWidth) * Size,(y * (m_nPathWidth + 1) + p) * Size,Size,Size,WHITE); // Draw East Passage
			}
		}
	}
	
	for (int py = 0; py < m_nPathWidth; py++)
		for (int px = 0; px < m_nPathWidth; px++)
			RenderRect((((Vic2*)Stack_Top(&m_stack))->x * (m_nPathWidth + 1) + px) * Size,(((Vic2*)Stack_Top(&m_stack))->y * (m_nPathWidth + 1) + py) * Size,Size,Size,GREEN);
}

void Delete(AlxWindow* w){
    
}

int main(){
    if(Create("Mazes!!!",1000,1025,1,1,Setup,Update,Delete))
        Start();
    return 0;
}