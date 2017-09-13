import java.util.concurrent.*;
public class Game_Run {
	public static void main(String args[])
	{
	      
	      Board p= new Board();
	      p.board_initially();
	      Refree p1 = new Refree(p);
	      PlayerClass c1 = new PlayerClass(1,p);
	      PlayerClass c2 = new PlayerClass(2,p);
	     ExecutorService executorService= Executors.newCachedThreadPool();
	     executorService.execute(p1);
	     executorService.execute(c1);
	     executorService.execute(c2);
	     executorService.shutdown();
	}

}
