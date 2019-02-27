

import junit.framework.TestCase;
import java.util.Random;
import main.java.com.mifmif.common.regex.Generex;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!





public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   public void testManualTest()
   {
	   //You can use this function to implement your manual testing	   
	   
   }
   
   
   public void testYourFirstPartition()
   {
	 //You can use this function to implement your First Partition testing	   

   }
   
   public void testYourSecondPartition(){
		 //You can use this function to implement your Second Partition testing	   

   }
   //You need to create more test cases for your Partitions if you need to 
   
   public static void randomTestValid()
   {
	 
	 //Establish random number generator instance
	 Random rand = new Random();
	 
	 //Establish StringBuilder object for URL generation
	 StringBuilder testUrl = new StringBuilder();
	 
	 //Randomly pick a valid scheme from the defaults
	 int randomScheme = rand.nextInt(3);
	 if(randomScheme == 0) testUrl.append("http://");
	 else if(randomScheme == 1) testUrl.append("https://");
	 else testUrl.append("ftp://");
	 
	 //Randomly generate a valid domain name...
	 
	 //Append random domain name characters (up to 63) onto testUrl, from set [{Alnum}.]. 
	 //(Max length for domain name is 63. See lines 73-75 of DomainValidator.java)
	 int randomDomainLength = 
			 	1 + rand.nextInt(63);
	 for(int i = 0, randomChar; i < randomDomainLength; i++) {
		 do{
			 randomChar = 46 + rand.nextInt(77);
		 }while((randomChar >= 58 && randomChar <= 64) ||
				(randomChar >= 91 && randomChar <= 96) ||
				(randomChar == 47));
		 if(i > 0 && testUrl.charAt(i-1) == '.' && randomChar == '.') {
			 do{
				 randomChar = 48 + rand.nextInt(75);
			 }while((randomChar >= 58 && randomChar <= 64) ||
					(randomChar >= 91 && randomChar <= 96));
		 }
		 testUrl.append((char)randomChar);
	 }
	 
	 //Append random top-level-domain from set of valid TDLs to testUrl...
	 
	 //Add seperator
	 testUrl.append(".");
	 
	 /*Abbreviations for improved readability... */
	 
	 //Infrastructure TLDS length
	 int iL = DomainValidatorReference.getInfrastructureTLDSLength();
	 
	 //Generic TLDS length
	 int gL = DomainValidatorReference.getGenericTLDSLength();
	 
	 //Country Code TLDS length
     int ccL = DomainValidatorReference.getCountryCodeTLDSLength();
     
     //Local TLDS length
   	 int lL = DomainValidatorReference.getLocalTLDSLength();
   	 
   	 /* End abbreviations assignments */
	 
	 //Get random TDL from set of total allowable TDLs
	 int randomTDL = rand.nextInt(iL + gL + ccL + lL);
	 
	 //Get TDL string according to randomTDL
	 if(randomTDL <= iL)
		 testUrl.append(DomainValidatorReference.getInfrastructureTLDS(randomTDL - 1));
	 
	 else if(randomTDL <= iL + gL)
		 testUrl.append(DomainValidatorReference.getGenericTLDS(randomTDL - iL - 1));
	 
	 else if(randomTDL <= iL + gL + ccL) 
		 testUrl.append(DomainValidatorReference.getCountryCodeTLDS(randomTDL - iL - gL - 1));
	
	 else
		 testUrl.append(DomainValidatorReference.getLocalTLDS(randomTDL - iL - gL - ccL - 1));
	 
	 //50% chance we use a port number...
	 if(rand.nextInt() % 2 == 0) {
		 testUrl.append(":");
		 testUrl.append(String.valueOf(rand.nextInt(65536)));
	 }
	 
	 //Use Generex to randomly generate a path string based on PATH_REGEX (UrlValidator.java line 167)
	 //Generex generex = new Generex("^(/[-\\w:@&?=+,.!*'%$_;\\(\\)]*)?$");
	 //Generex generex = new Generex("^(/[abcdefghijklmnopqrstuv]*)?$");
	 Generex generex = new Generex("/[/A-Za-z0-9-_@&?=+,.!*'%$;\\(\\):]*");
	 String randomPath = generex.random(rand.nextInt(80));		//80 char max
	 testUrl.append(randomPath);
	 
	 System.out.println(testUrl.toString());
	 System.out.println(randomPath);
   }
   
   public void testIsValid()
   {
	   //You can use this function for programming based testing

   }
   
   public static void main(String[] args) {
	   
	   UrlValidatorTest.randomTestValid();
	   
   }

}

// References
// https://stackoverflow.com/questions/4669692/valid-characters-for-directory-part-of-a-url-for-short-links
// https://docs.oracle.com/javase/7/docs/api/java/util/regex/Pattern.html

