

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
   
   public static void randomTestValid(int testRuns)
   {
	 
	 //Instantiate random number generator
	 Random rand = new Random();
	 
	 //Instantiate url validator
	 UrlValidator urlValidator = new UrlValidator();
	 
	 //Establish StringBuilder object for URL generation
	 StringBuilder testUrlSB = new StringBuilder();
	 
	 for(int i = 0; i < 100000; i++) {
		 testUrlSB.setLength(0);
		 
		 //Randomly pick a valid scheme from the defaults
		 int randomScheme = rand.nextInt(3);
		 if(randomScheme == 0) testUrlSB.append("http://");
		 else if(randomScheme == 1) testUrlSB.append("https://");
		 else testUrlSB.append("ftp://");
		 
		 //Randomly generate a valid domain name...
		 
		 //Append random domain name characters (up to 63) onto testUrlSB, from set [{Alnum}.]. 
		 //(Max length for domain name is 63. See lines 73-75 of DomainValidator.java)
		 int randomDomainLength = 
				 	1 + rand.nextInt(63);
		 for(int j = 0, randomChar; j < randomDomainLength; j++) {
			 do{
				 randomChar = 45 + rand.nextInt(78);
			 }while((randomChar >= 58 && randomChar <= 64) ||
					(randomChar >= 91 && randomChar <= 96) ||
					(randomChar == 47));
			 
			 //Make sure a '.' is not followed by another '.', 
			 //'-' is not followed by another '-', 
			 //and that the first character of the authority is not a '.'
			 if((j > 0 && testUrlSB.charAt(j-1) == '.' && randomChar == 46) ||
				(j > 0 && testUrlSB.charAt(j-1) == '+' && (char)randomChar == '+') ||
				(j == 0 && (char)randomChar == '.')) {
				 do{
					 randomChar = 48 + rand.nextInt(75);
				 }while((randomChar >= 58 && randomChar <= 64) ||
						(randomChar >= 91 && randomChar <= 96));
			 }
			 testUrlSB.append((char)randomChar);
		 }
		 
		 //Append random top-level-domain from set of valid TLDs to testUrlSB...
		 
		 //Add separator if needed (i.e. last character of authority is not already '.'
		 if(testUrlSB.charAt(testUrlSB.length() - 1) != '.') {
			 testUrlSB.append(".");
		 }
		 
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
	   	 
		 //Get random TLD from set of total allowable TLDs
		 int randomTLD = rand.nextInt(iL + gL + ccL + lL);
		 
		 //Get TLD string according to randomTLD
		 if(randomTLD < iL)
			 testUrlSB.append(DomainValidatorReference.getInfrastructureTLDS(randomTLD));
		 
		 else if(randomTLD < iL + gL)
			 testUrlSB.append(DomainValidatorReference.getGenericTLDS(randomTLD - iL));
		 
		 else if(randomTLD < iL + gL + ccL) 
			 testUrlSB.append(DomainValidatorReference.getCountryCodeTLDS(randomTLD - iL - gL));
		
		 else
			 testUrlSB.append(DomainValidatorReference.getLocalTLDS(randomTLD - iL - gL - ccL));
		 
		 //50% chance we use a port number...
		 if(rand.nextInt() % 2 == 0) {
			 testUrlSB.append(":");
			 testUrlSB.append(String.valueOf(rand.nextInt(65536)));
		 }
		 
		 //75% chance we use a path...
		 if((1 + rand.nextInt(4)) % 3 != 0) {
			 //Use Generex to randomly generate a path string based on PATH_REGEX 
			 //(see UrlValidator.java line 167). Max length for random path is 80.
			 Generex generex = 
					 new Generex("(/[-A-Za-z0-9:@&=+,!*'$_;\\(\\)]*(%[A-Fa-f0-9]{2})?(\\.)?)+");
			 String randomPath = generex.random(rand.nextInt(80));		//80 char max
			 testUrlSB.append(randomPath);
			 //System.out.println("Path: " + randomPath);
		 }
		 
		 //50% chance we use a query...
		 if(rand.nextInt() % 2 == 0) {
			 //Use Generex to randomly generate a path string based on PATH_REGEX 
			 //(see UrlValidator.java line 167). Max length for random path is 80.
			 Generex generex = new Generex("?\\w+=\\w+(&\\w+=\\w+)*");
			 String randomQuery = generex.random(rand.nextInt(150));	//150 char max
			 testUrlSB.append(randomQuery);
			 //System.out.println("Query: " + randomQuery);
		 }
		 
		 //Covert testUrlSB string builder to a string
		 String testUrl = testUrlSB.toString();
		 
		 //System.out.println(String.valueOf(i) + "    " + testUrl);
		 
		 //Run URL validation check
		 //assertFalse(testUrl, urlValidator.isValid(testUrl));
		 //assertFalse(testUrl, urlValidator.isValid("www.google.com"));
		 if(urlValidator.isValid(testUrl) == false) {
			 System.out.println(String.valueOf(i) + "    " + "false" + "    " + testUrl);
		 }
		 if(urlValidator.isValid(testUrl) == true) {
			 System.out.println(String.valueOf(i) + "    " + "true" + "    " + testUrl);
		 }
	 }
	 
   }
   
   public void testIsValid()
   {
	   //You can use this function for programming based testing

   }
   
   public static void main(String[] args) {
	   
	   UrlValidatorTest.randomTestValid(1000);
	   
   }

}

// References
// https://stackoverflow.com/questions/4669692/valid-characters-for-directory-part-of-a-url-for-short-links
// https://docs.oracle.com/javase/7/docs/api/java/util/regex/Pattern.html
// https://github.com/cs-au-dk/dk.brics.automaton
// http://junit.sourceforge.net/javadoc/org/junit/Assert.html#assertFalse(java.lang.String,%20boolean)
// https://stackoverflow.com/questions/5192512/how-can-i-clear-or-empty-a-stringbuilder
