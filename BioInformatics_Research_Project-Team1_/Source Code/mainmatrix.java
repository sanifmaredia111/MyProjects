
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Tejaswi Bharadwaj
 */
public class mainmatrix {



/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Sanif Maredia
 */

 
 public static void main(String[] args) throws IOException {
     
     File file = new File("C:/Users/tejas/Desktop/workspace/code/genedb.csv");
     if (!file.exists())
     {
	file.createNewFile();
     }
     FileWriter fw = new FileWriter(file.getAbsoluteFile());
     BufferedWriter bw = new BufferedWriter(fw);
  FileReader fileReader = new FileReader("C:/Users/tejas/Desktop/workspace/code/Data_set.txt");
    
  String fileContents = "";
 bw.flush();
  int i,count,colcnt ;
 count=1;
 colcnt=1;
  while((i =  fileReader.read())!=-1){
   
      char ch = (char)i;
      fileContents = fileContents + ch; 
      
    if(count%3==0)
    {
  System.out.print(fileContents+ ",");
  bw.write(fileContents+",");    
  fileContents="";
      colcnt++;
      if(colcnt>64)
      {
          System.out.print("\n");
          bw.write("\n");   
          colcnt=1;
      }
    }
   
  count++;
  // fileContents = fileContents + ch + ","; 
   
   
  }
 
  bw.close();
 }
}
