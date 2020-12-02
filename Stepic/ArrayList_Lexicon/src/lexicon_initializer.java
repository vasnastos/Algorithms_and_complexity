import java.util.Random;
import java.util.Vector;

import javax.swing.JOptionPane;

public class lexicon_initializer {
	public static void main(String[] args) {
		String htmloutcome="<html><h3 style=\"text-align:center;\">RESULTS</h3><hr><ol>";
        Lexicon l=new Lexicon();
        htmloutcome+="<li>Read Data</li><ul>";
        long starttime=System.currentTimeMillis();
        l.read_data();
        long estimated_time=System.currentTimeMillis()-starttime;
        htmloutcome+="<li>Total data read:"+String.valueOf(l.size())+"</li><li>Time estimated:"+String.valueOf(estimated_time)+"</li>";
        htmloutcome+="</ul><li>Choose Randomly Data</li>";
        Random r=new Random();
        starttime=System.currentTimeMillis();
        Vector <String> words=l.getRandomData(r.nextInt(l.size()));
        htmloutcome+="<ul><li>Data fetched:"+String.valueOf(words.size())+"</li>";
        estimated_time=System.currentTimeMillis()-starttime;
        htmloutcome+="<li>Time Established:"+String.valueOf(estimated_time)+"</li></ul>";
        htmloutcome+="<li>Find Data</li><ul>";
        int passcount=0,failcount=0;
        starttime=System.currentTimeMillis();
        for(String x:words)
        {
        	if(l.find(x))
        	{
        		passcount++;
        	}
        	else 
        	{
        		failcount++;
        	}
        }
        estimated_time=System.currentTimeMillis()-starttime;
        htmloutcome+="<li>Data Searched:"+String.valueOf(words.size())+"</li>";
        htmloutcome+="<li>Words found:"+String.valueOf(passcount)+"</li><li>Words not found:"+String.valueOf(failcount)+"</li>";
        htmloutcome+="<li>Estimated Time:"+String.valueOf(estimated_time)+"</li></ul>";
        htmloutcome+="<li>Removes</li><ul>";
        starttime=System.currentTimeMillis();
        for(String x:words)
        {
        	l.remove(x);
        }
        estimated_time=System.currentTimeMillis()-starttime;
        htmloutcome+="<li>Words Removed:"+String.valueOf(words.size())+"</li>";
        htmloutcome+="<li>Estimated time:"+String.valueOf(estimated_time)+"</li></ul>";
        htmloutcome+="<li>Insert Words</li><ul>";
        starttime=System.currentTimeMillis();
        for(String x:words)
        {
        	l.insert(x);
        }
        estimated_time=System.currentTimeMillis()-starttime;
        htmloutcome+="<li>Words Inserted:"+String.valueOf(words.size())+"</li>";
	    htmloutcome+="<li>Time Estimated:"+String.valueOf(estimated_time)+"</li></ul>";
	    htmloutcome+="</ol></html>";
	    JOptionPane.showMessageDialog(null, htmloutcome);
	}

}
