#include<iostream>
#include<vector>
//min-max algorithm for tic-tac game
#define character(x) x%2==0?'O':'X'
using std::vector;
//Defination of the tree created to analysis all the possible state.
class TreeNode{
public:
    vector<vector<char>>grid; //data to be stored in the tree.
    vector<TreeNode*>child;//child nodes
    int row,column;
    int label;// crucial role, the leaf nodes are assigned with the terminating label .
    TreeNode(){}
    TreeNode(vector<vector<char>>_grid):label(-100){
        grid=_grid;
    }
    //marking the current row and column.
    void mark(int move){
        grid[row][column]=character(move);
    }
    //note memory not cleared.
};
//check for win or draw state. terminating state
bool Condition(TreeNode*root,char symbol){
   vector<vector<char>>grid=root->grid;
   //checking row 
   if(grid[root->row][0]==symbol && grid[root->row][1]==symbol && grid[root->row][2]==symbol){
    root->label=(symbol=='O')?-1:1;
    return true;
   }//checking the column
   else if(grid[0][root->column]==symbol && grid[1][root->column]==symbol && grid[2][root->column]==symbol){
    root->label=(symbol=='O')?-1:1;
    return true;
   }//checking the diagonal.
   else if(grid[0][0]==symbol && grid[1][1]==symbol && grid[2][2]==symbol)
   {
    root->label=(symbol=='O')?-1:1;
    return true;
   }
   else if(grid[0][2]==symbol && grid[1][1]==symbol && grid[2][0]==symbol)
   {
    root->label=(symbol=='O')?-1:1;
    return true;
   }
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
           if(grid[i][j]==' ')
            return false; 
        }
    }
    root->label=0;
    return true;
}

void Display_Board(vector<vector<char>>&grid){
    for(int i=0;i<grid.size();i++)
    {
        for(int j=0;j<grid[0].size();j++)
        {
            std::cout<<" "<<grid[i][j];
            if(j !=2)
            std::cout<<" |";
        }
        std::cout<<std::endl;
        if(i!=2)
        std::cout<<"--- --- ---"<<std::endl;
    }
}
//frontend.
void play_Game(vector<vector<char>>&grid,int &move,TreeNode*);

TreeNode*Build_Tree(TreeNode*,vector<vector<char>>,int);
//evaluate function.
int Min_MAX(TreeNode* root,bool);
TreeNode* Search(vector<TreeNode*>,int);
TreeNode*Search(vector<TreeNode*>,int row,int column);
int main()
{
    //start default play 
   vector<vector<char>>grid(3,vector<char>(3,' '));
    int move=0;
    int row,column;
    std::cout<<"Default. player - O and computer X"<<std::endl;
    std::cout<<"input 0 to 2 indexing similar to 2d array.\n";
    std::cout<<"Enter the row- ";
    std::cin>>row;
    std::cout<<"Enter the column- ";
    std::cin>>column;
    grid[row][column]=character(move++);
    Display_Board(grid);
    std::cout<<"computer\n";
    if(grid[1][1]==' ')
        grid[1][1]=character(move++);
    else 
        grid[0][0]=character(move++);
    Display_Board(grid);
    do{
        std::cout<<"Enter the row- ";
        std::cin>>row;
        std::cout<<"Enter the column- ";
        std::cin>>column;
    }while(grid[row][column]!=' ');
    grid[row][column]=character(move++);
    Display_Board(grid);
    TreeNode* root=new TreeNode(grid);
    root->row=row;root->column=column;
    //actual logic for the Computer.
    play_Game(grid,move,root);
    return 0;
}

void play_Game(vector<vector<char>>&grid,int &move,TreeNode* root)
{ 
    int row,column;
    root=Build_Tree(root,root->grid,move);
   do
   {
    int next_move=Min_MAX(root,true);
    root=Search(root->child,next_move);
    grid=root->grid;
    std::cout<<"\nComputer move:\n";

    Display_Board(grid);
    if(root->child.empty())break;
    do{
        std::cout<<"Enter the row- ";
        std::cin>>row;
        std::cout<<"Enter the column- ";
        std::cin>>column;
    }while(grid[row][column]!=' ');
    std::cout<<"\nUser-move\n";
    root=Search(root->child,row,column);
    grid=root->grid;
    Display_Board(grid);
   } while (!root->child.empty());
   //check the final state.
   int status=root->label;
   if(status==1){
    std::cout<<"The game is won by the computer.\n";
   }
   else if(status==-1)
   std::cout<<"The game is won by you.";
    else
    std::cout<<"The game is draw.\n";
    std::cout<<std::endl;
}
//building a one time tree for all possible moves when the bord is filled from 3 states.
TreeNode*Build_Tree(TreeNode*root,vector<vector<char>>state,int move)
{
    //win || loss || draw.
    int symbol=move-1;
    if(Condition(root,character(symbol)))
        return root;
        for(int i=0;i<state.size();i++)
        {
            for(int j=0;j<state[0].size();j++)
            {
                if(state[i][j]==' '){
                    TreeNode* curr=new TreeNode(state);
                    curr->row=i;curr->column=j;
                    curr->mark(move);
                    root->child.push_back(curr);
                }
            }
        }
        for(int i=0;i<root->child.size();i++){
            root->child[i]=Build_Tree(root->child[i],root->child[i]->grid,move+1);  
        }
    
   
    return root;
}
//backend for evaluating the next possible optimal move.
int Min_MAX(TreeNode* root,bool maximizer)
{
  if (root->child.empty()) return root->label;
  if(maximizer)
  {
    int max=INT_MIN;
    for(auto child:root->child){
        root->label=max=std::max(max,Min_MAX(child,false));
    }
    return max;
  }
  else{
    int min=INT_MAX;
    for(auto child:root->child)
    {
        root->label=min=std::min(min,Min_MAX(child,true));
    }
    return min;
  }

}
//search value for the computer.
TreeNode* Search(vector<TreeNode*>child,int next)
{
    TreeNode*root=NULL;
    for(auto x:child)
    {
        if(x->label==next){
            root=x;
            break;
        }
    }
    return root;
}
//search value for the user.
TreeNode* Search(vector<TreeNode*>child,int row,int column)
{
    TreeNode*root=NULL;
    for(auto x:child)
    {
        if(x->row==row && x->column==column){
            root=x;
            break;
        }
    }
    return root;
}

/*
Note:
Here i used 'O' for the user move and 'X' is used by the computer, a one time tree is build where both the user and computer relay on it 
Evaluation is used by the MINMAX algorithm for making the decision.
'X' is consider as maximizing player with its win state +1 and 'O' is consider as minimizing player with its win state -1.
Algorithm is used by 'X' only so it always try to evaluate the maximum value as far as possible either 0 or 1


*/