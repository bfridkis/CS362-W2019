

import junit.framework.TestCase;
import java.util.Random;
import main.java.com.mifmif.common.regex.Generex;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!





public class UrlValidatorTest extends TestCase {

   private static int RANDOM_TEST_RUNS = 100000;

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
	 
	 //Instantiate random number generator
	 Random rand = new Random();
	 
	 //Instantiate url validator
	 UrlValidator urlValidator = new UrlValidator(UrlValidator.ALLOW_LOCAL_URLS);
	 
	 //Establish StringBuilder object for URL generation
	 StringBuilder testUrlSB = new StringBuilder();
	 
	 //Declare Generex Object
	 Generex generex;
	 
	 int falsePrev = 0;
	 
	 for(int i = 0; i < RANDOM_TEST_RUNS; i++) {
		 testUrlSB.setLength(0);
		 
		//Randomly pick a valid scheme from the defaults
		 int randomScheme = rand.nextInt(3);
		 if(randomScheme == 0) testUrlSB.append("http://");
		 else if(randomScheme == 1) testUrlSB.append("https://");
		 else testUrlSB.append("ftp://");
		 
		 /* One-Third of the time use hostname for authority */
		 int randomAuth = rand.nextInt(3) + 1;
			 if(randomAuth % 3 == 1) {
			 
			//Randomly generate a valid domain name...
			 
//			 //Manual method for generating domain name (i.e. without Generex)...
//			 //Append random domain name characters (up to 63) onto testUrlSB, from set [{Alnum}.]. 
//			 //(Max length for domain name is 63. See lines 73-75 of DomainValidator.java)
//			 int randomDomainLength = 1 + rand.nextInt(63), randomChar = 0;
//			 for(int j = 0, lastIndex; j < randomDomainLength; j++) {
//				 do{
//					 randomChar = 45 + rand.nextInt(78);
//				 }while((randomChar >= 58 && randomChar <= 64) ||
//						(randomChar >= 91 && randomChar <= 96) ||
//						(randomChar == 47));
//				 
//				 //Make sure a '.' is not followed by another '.' or a '-', 
//				 //'-' is not followed by another '-' or '.', 
//				 //and the first character of the authority is not a '.' nor '-'.
//				 lastIndex = testUrlSB.length() - 1;
//				 if((j > 0 && testUrlSB.charAt(lastIndex) == '.' && 
//						 	((char)randomChar == '.' || (char)randomChar == '-')) ||
//					(j > 0 && testUrlSB.charAt(lastIndex) == '-' && 
//						((char)randomChar == '-' || (char)randomChar == '.')) ||
//					(j == 0 && (char)randomChar == '.') ||
//					(j == 0 && (char)randomChar == '-')) {
//					 do{
//						 randomChar = 48 + rand.nextInt(75);
//					 }while((randomChar >= 58 && randomChar <= 64) ||
//							(randomChar >= 91 && randomChar <= 96));
//				 }
//				 testUrlSB.append((char)randomChar);
//			 }
			 
			 //Use Generex to append random domain name characters (up to 63) onto testUrlSB. 
			 //(Max length for domain name [without TLD] is 63. See lines 73-75 of DomainValidator.java)
			 generex = new Generex("([A-Za-z0-9]+[\\-\\.]?)+");
			 String randomAuthority = generex.random(rand.nextInt(64));
			 //May still exceed 63 characters based on last pattern match, so truncate if necessary
			 if(randomAuthority.length() > 63) randomAuthority = randomAuthority.substring(0, 63);
			 testUrlSB.append(randomAuthority);
			 //System.out.println("Authority: " + randomAuthority);

			 
			 
			 //Replace last character of authority if == '-' or '.' 
			 //(so "-." and ".." sequences are prevented)
			 if(testUrlSB.charAt(testUrlSB.length() - 1) == '-' ||
				testUrlSB.charAt(testUrlSB.length() - 1) == '.') {
				 int randomChar = 0;
				 do{
					 randomChar = 48 + rand.nextInt(75);
				 }while((randomChar >= 58 && randomChar <= 64) ||
						(randomChar >= 91 && randomChar <= 96));
				 testUrlSB.replace(testUrlSB.length() - 1, testUrlSB.length(), 
			 				String.valueOf((char)randomChar));
			 }
			 	
			 
			 //Add separator if needed (i.e. last character of authority is not already '.')
			 if(testUrlSB.charAt(testUrlSB.length() - 1) != '.') {
				 testUrlSB.append(".");
			 }
			 
			 //Append random top-level-domain from set of valid TLDs to testUrlSB...
			 
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
		 }
			 
		 /* One-Third of the time use IPV4 for authority */
		 else if(randomAuth % 3 == 2) {
			 //Use Generex to append random ip4v address onto testUrlSB. 
			 generex = new Generex("([1-2]?[1-5]?[0-5]\\.){3}([1-2]?[1-5]?[0-5]){1}");
			 String randomAuthority = generex.random();
			 testUrlSB.append(randomAuthority);
			 //System.out.println("Authority: " + randomAuthority);
		 }
		 
		 /* One-Third of the time use IPV6 for authority */
		 else {
			//Use Generex to append random ipv6 address onto testUrlSB...
			
			//Use compression 50% of the time
			if(rand.nextInt()% 2 == 0)
					//No compression
					generex = new Generex("\\[([0-9a-fA-F]{4}:){7}[0-9a-fA-F]{4}\\]");
			else {
				//Compression
				int numCompHextets = 1 + rand.nextInt(7);
				if(numCompHextets == 7) {
					if(rand.nextInt() % 2 == 0)
						generex = new Generex("\\[::[0-9a-fA-F]{4}\\]");
					else
						generex = new Generex("\\[[0-9a-fA-F]{4}::\\]");
				}
				else {
					//Compression position starts at 1
					int compPos = rand.nextInt(7 - numCompHextets) + 1;
					generex = new Generex("\\[([0-9a-fA-F]{4}:){"+String.valueOf(compPos)+"}" +
										  ":([0-9a-fA-F]{4}:){"+String.valueOf(7-numCompHextets-compPos)+"}" +
										  "[0-9a-fA-F]{4}\\]");
					
				}
			}
			String randomAuthority = generex.random();
			testUrlSB.append(randomAuthority);
			//System.out.println("Authority: " + randomAuthority);
		 }
			 
		 //50% chance we use a port number...
		 if(rand.nextInt() % 2 == 0) {
			 testUrlSB.append(":");
			 testUrlSB.append(String.valueOf(rand.nextInt(65536)));
		 }
		 
		 //75% chance we use a path...
		 if((1 + rand.nextInt(4)) % 3 != 0) {
			 //Use Generex to randomly generate a path string based on PATH_REGEX 
			 //(see UrlValidator.java line 167). Max length for random path is 80.
			 generex = 
					 new Generex("(/[-A-Za-z0-9:@&=+,!*'$_;\\(\\)]+(%[A-Fa-f0-9]{2})?(\\.)?)+");
			 String randomPath = generex.random(rand.nextInt(81));		//80 char max
			 //May still exceed 80 characters based on last pattern match, so truncate if necessary
			 if(randomPath.length() > 80) randomPath = randomPath.substring(0, 81);
			 testUrlSB.append(randomPath);
			 //System.out.println("Path: " + randomPath);
		 }
		 
		 //50% chance we use a query...
		 if(rand.nextInt() % 2 == 0) {
			 //Use Generex to randomly generate a path string based on PATH_REGEX 
			 //(see UrlValidator.java line 167). Max length for random path is 80.
			 generex = new Generex("?\\w+=\\w+(&\\w+=\\w+)*");
			 String randomQuery = generex.random(rand.nextInt(151));	//150 char max
			//May still exceed 63 characters based on last pattern match, so truncate if necessary
			 if(randomQuery.length() > 150) randomQuery = randomQuery.substring(0, 151);
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
			 //System.out.println("Authority: " + randomAuthority);
			 System.out.println(String.valueOf(i) + "    " + "false" + "    " + testUrl);
			 falsePrev = 1;
		 }
//		 if(urlValidator.isValid(testUrl) == true && falsePrev == 1) {
//			 System.out.println(String.valueOf(i) + "    " + "true" + "    " + testUrl);
//			 falsePrev = 0;
//		 }
	 }
	 
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
// https://github.com/cs-au-dk/dk.brics.automaton
// http://junit.sourceforge.net/javadoc/org/junit/Assert.html#assertFalse(java.lang.String,%20boolean)
// https://stackoverflow.com/questions/5192512/how-can-i-clear-or-empty-a-stringbuilder
// https://stackoverflow.com/questions/8499698/trim-a-string-based-on-the-string-length/8499776
// https://softwareengineering.stackexchange.com/questions/311413/how-should-we-represent-an-ipv6-address-with-port-number-in-text
// https://community.cisco.com/t5/ipv6/what-do-you-call-the-ipv6-address-parts/td-p/1690102
