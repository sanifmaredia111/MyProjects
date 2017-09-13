
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Sanif Maredia
 */  
public class combination {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws IOException {
        String[][] allcombi = new String[801][64];
        int[][] rescombi = new int[801][64];
        String[] a= {"A","C","G","T"};
        String[] c= {"A","C","G","T"};
        String[] g= {"A","C","G","T"};
        String[] t= {"A","C","G","T"};
        int i,j,k,l,cnt=0;
        //String [][] readdataval = new String[16][64];
        Scanner scanner = new Scanner("C:/Users/tejas/Desktop/workspace/code/genedb.csv");
        
        File file = new File("C:/Users/tejas/Desktop/workspace/code/maincombi.csv");
        
        BufferedReader readdataval = new BufferedReader(new FileReader(new File("C:/Users/tejas/Desktop/workspace/code/genedb.csv")));
        String line = "";
        i=0;
        
        

         
     if (!file.exists())
     {
	file.createNewFile();
     }
     FileWriter fw = new FileWriter(file.getAbsoluteFile());
     BufferedWriter bw = new BufferedWriter(fw);
     bw.flush();
     for(i=0;i<4;i++)
     {
         for(j=0;j<4;j++)
         {
             for(k=0;k<4;k++)
             {
                
                  allcombi[0][cnt]=a[i]+c[j]+g[k]+",";
                  System.out.print(allcombi[0][cnt]);
                  bw.write(allcombi[0][cnt]);
                  cnt++;
                 
             }
         }
     }
       for(i=1;i<801;i++)
     {
          bw.write("\n");
         for(j=0;j<64;j++)
         {
           
             allcombi[i][j]="0"+",";
             bw.write(allcombi[i][j]);
         }
     }
    
     bw.close();
    
     
     
    }
    
}
