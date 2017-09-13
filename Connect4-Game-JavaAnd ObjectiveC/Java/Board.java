import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.security.SecureRandom;
import java.util.concurrent.locks.Condition;
public class Board {
	
	private final Lock accessLock = new ReentrantLock();
	private final Condition canPlay = accessLock.newCondition();
	private final Condition canCheck = accessLock.newCondition();
	private SecureRandom randomNumbers= new SecureRandom();
	private static int turn=0;
	private static int game_over=0;
	private static int a[][] = new int[6][7];
	private static final int rows=6,cols=7;
	private int player_no;
    
	public void board_initially()
	{
		 int i,j;
		 for(i=0;i<6;i++)
		    {
		      for(j=0;j<7;j++)          //filling the board with 0 initially
		        {
		          a[i][j]=0;
		        }
		    }
	}
	
	
	public void player_play(int player_no) throws InterruptedException //play method for players
	{
		this.player_no = player_no;
		
		try{
			int b,i,j;
			  
			  for(;;)
			    {   
				    accessLock.lock();
			      while(turn==0)
			        canPlay.await();
			      if(game_over==1)
			        {
			          canCheck.signalAll();
			          accessLock.unlock();
			          return;
			        }
			     Thread.sleep(1000);
			      

			      b=randomNumbers.nextInt(6);
			      
			      
			      for(i=5;i>=0;i--)           //insertion operation in board by player
			        {
			          if(a[i][b]==0)
			            {
			        	  a[i][b]=player_no;
			        	  System.out.println("\n");
			              System.out.println("Board display after " +a[i][b]+" chance");
			              System.out.println("\n");
			              break;
			            }
			        }
			     
			      for(i=0;i<rows;i++)         //Display Operation of board after player chance
			        {

			          for(j=0;j<cols;j++)
			            {
			        	  System.out.print(+a[i][j]+"  ");
			            }
			              System.out.println();
			        }
			      turn=0;
			      canCheck.signalAll();
			      accessLock.unlock();
			}

			
		}
	
		finally{
			//accessLock.unlock();
			
		}
		
		
	}

	
	public void refree_Check() throws InterruptedException  // refree will check board
	{
		  int i,j,m,count2;
		
	  try{
		  for(;;)
		    {
			  
			  accessLock.lock();

		      while(turn==1)
		    	  canCheck.await();    //refree wait for player to play

		      turn=1;

		      count2=0;
		      for ( i= 0; i < rows; ++i)     //check for horizontal for winner
		        {
		          int count = 0;

		          for (j = 1; j < cols; ++j)
		            {
		              if (a[i][j] != 0 && a[i][j] == a[i][j-1])
		                ++count;
		              else
		                count = 1;

		              if (count >= 4)
		                {
		                  game_over=1;
		                 System.out.printf(" Player %d  Won ! Match:Horizontal\n", a[i][j]);
		                  
		                  canPlay.signalAll();
		              
		                  accessLock.unlock();
		                  return;
		                }
		            }
		        }
		      int b=0;
		      int n;
		      for(n=1; n<=2; n++)  //diagonal check for winner
		        {

		          if((a[3][b+2]==n && a[2][b+3]==n && a[1][b+4]==n && a[0][b+5]==n) ||
		             (a[5][b]==n && a[4][b+1]==n && a[3][b+2]==n && a[2][b+3]==n) ||
		             (a[4][b+1]==n && a[3][b+2]==n && a[2][b+3]==n && a[1][b+4]==n) ||
		             (a[3][b+3]==n && a[2][b+4]==n && a[1][b+5]==n && a[0][b+6]==n) ||
		             (a[5][b+1]==n && a[4][b+2]==n && a[3][b+3]==n && a[2][b+4]==n) ||
		             (a[5][b+2]==n && a[4][b+3]==n && a[3][b+4]==n && a[2][b+5]==n) ||
		             (a[5][b+3]==n && a[4][b+4]==n && a[3][b+5]==n && a[2][b+6]==n) ||
		             (a[4][b]==n  && a[3][b+1]==n && a[2][b+2]==n && a[1][b+3]==n)  ||
		             (a[3][b+1]==n && a[2][b+2]==n && a[1][b+3]==n && a[0][b+4]==n) ||
		             (a[3][b]==n  && a[2][b+1]==n && a[1][b+2]==n && a[0][b+3]==n))
		            {
		              game_over=1;
		              System.out.println(" Player " +n+ "Won! Match:Diagonal\n");
		              canPlay.signalAll();
		              accessLock.unlock();
		               
		              return;

		            }

		          if((a[3][b+2]==n && a[2][b+3]==n && a[1][b+4]==n && a[0][b+5]==n) ||
		             (a[5][b]==n && a[4][b+1]==n && a[3][b+2]==n && a[2][b+3]==n)  ||
		            (a[4][b+1]==n && a[3][b+2]==n && a[2][b+3]==n && a[1][b+4]==n) ||
		              (a[3][b+3]==n && a[2][b+4]==n && a[1][b+5]==n && a[0][b+6]==n) ||
		              (a[5][b+1]==n && a[4][b+2]==n && a[3][b+3]==n && a[2][b+4]==n) ||
		              (a[4][b+2]==n && a[3][b+3]==n && a[2][b+4]==n && a[1][b+5]==n) ||
		              (a[0][b+2]==n && a[1][b+3]==n && a[2][b+4]==n && a[3][b+5]==n) ||
		              (a[0][b+3]==n && a[1][b+4]==n && a[2][b+5]==n && a[3][b+6]==n) ||
		              (a[2][b]==n && a[3][b+1]==n && a[4][b+2]==n && a[5][b+3]==n)  ||
		        (a[2][b+2]==n && a[3][b+3]==n && a[4][b+4]==n && a[5][b+5]==n))
		    {
		      game_over=1;
		      System.out.println(" Player " +n+ "Won! Match:Diagonal\n");
		      canPlay.signalAll();
             accessLock.unlock();
		      return;
		    }
		}
		    for (m=0; m < cols; ++m)    //board check whether it is empty or not
		      {
		        if(a[0][m]!=0)
		          {
		            ++count2;
		          }
		      }

		    if (count2 >= 7)      //check if it is tie
		      {
		        game_over=1;
		        System.out.println("Board is full, Game result: Tie \n");
		        canPlay.signalAll();
	          accessLock.unlock();
			    return;
		      }
		    canPlay.signalAll();
          accessLock.unlock();
		}

	}

	  finally{
			//accessLock.unlock();
			
		}
}
}
	
	
	
