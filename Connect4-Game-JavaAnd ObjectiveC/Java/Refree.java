
public class Refree implements Runnable {
	//private Board player;
	private Board board;
	public Refree(Board board)
	{
		this.board=board;
		//this.player=player;
	}
	public void run()
	{
		try{
			
		
		board.refree_Check();
		}
       catch(InterruptedException exception)
       {
    	 exception.printStackTrace();
    	 Thread.currentThread().interrupt();
       }
	}

}
