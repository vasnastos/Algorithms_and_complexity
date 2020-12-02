import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Vector;

import javax.swing.JFileChooser;

public class Lexicon {
   List <String> lex;
   public Lexicon()
   {
	   lex=new ArrayList<String>();
   }
   public void read_data()
   {
	   JFileChooser ch=new JFileChooser();
	   int rval=ch.showOpenDialog(null);
	   if(rval==JFileChooser.APPROVE_OPTION)
	   {
		   String filename=ch.getSelectedFile().getAbsolutePath();
		   try {
			FileReader fr=new FileReader(filename);
			BufferedReader br=new BufferedReader(fr);
			try {
				String line=br.readLine();
				while(line!=null)
				{
					lex.add(line);
					line=br.readLine();
				}
				br.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	   }
   }
   private String shuffle(String word)
   {
   	Random r=new Random();
   	String shuffleword="";
   	for(int i=0;i<word.length();i++)
   	{
   		shuffleword+=word.charAt(r.nextInt(word.length()));
   	}
   	return shuffleword;
   }
   public boolean find(String word)
   {
	   return this.lex.contains(word);
   } 
   public void insert(String word)
   {
	   this.lex.add(word);
   }
   public void remove(String word)
   {
	   this.lex.remove(word);
   }
   public int size()
   {
	   return this.lex.size();
   }
   public Vector <String> getRandomData(int length)
   {
	   Vector <String> data=new Vector<String>();
	   Random r=new Random();
	   for(int i=0,t=this.lex.size();i<t;i++)
	   {
		   String newword=this.shuffle(this.lex.get(r.nextInt(length)));
		   data.add(newword);
	   }
	   return data;
   }
}
