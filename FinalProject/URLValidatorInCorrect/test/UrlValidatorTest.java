

import junit.framework.TestCase;
import java.util.Random;
import main.java.com.mifmif.common.regex.Generex;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!





public class UrlValidatorTest extends TestCase {

   private static int RANDOM_TEST_RUNS = 1000;
   
   //Constants corresponding to sabotageMask
   private static int SABOTAGE_SCHEME = 1 << 0;
   private static int SABOTAGE_AUTHORITY = 1 << 1;
   private static int SABOTAGE_PORT = 1 << 2;
   private static int SABOTAGE_PATH = 1 << 3;
   private static int SABOTAGE_QUERY = 1 << 4;

   public UrlValidatorTest(String testName) {
      super(testName);
   }
	
   public static void testIsValidUnit()
   {
	   //Establish StringBuilder object for URL generation
	   StringBuilder testUrl = new StringBuilder();
	   
	   //Configure UrlValidator object
	   long options = UrlValidator.ALLOW_ALL_SCHEMES;
	   UrlValidator validator = new UrlValidator(null, null, options);
	   
	   boolean valid = true;
	   boolean result;
	   
	   //Create matrices for each URL component
	   String[] schemes = {"http://", "https://", "ftp://", "qa>z://", ""};
	   String[] authorities = {"google.com", "wikipedia.org", "abcd.edu", "1.2.3.4", "255.0.0.255", "[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]", "[1080:0:0:0:8:800:200C:417A]", "412.365.0.27", "[23G:1:B7:70C:B9E1:7BA3:93:3B47]", ""};
	   String[] ports = {"", ":1", ":65535", ":-1", ":92834927"};
	   String[] paths = {"", "/index", "/?test=1", "/?test=1&test2=2", "/index?test=1"};
	   
	   //Loop through each matrix
	   for(int a=0; a<schemes.length; a++) {
		   for(int b=0; b<authorities.length; b++) {
			   for(int c=0; c<ports.length; c++) {
				   for(int d=0; d<paths.length; d++) {
					   
					   //Piece together the full URL to test
					   testUrl.append(schemes[a]);
					   testUrl.append(authorities[b]);
					   testUrl.append(ports[c]);
					   testUrl.append(paths[d]);
					   
					   //Determine expected result
					   if(a<3 && b<7 && c<3) {
						   valid = true;
					   }
					   else {
						   valid = false;
					   }
					   
					   //Test the URL with isValid()
					   result = validator.isValid(testUrl.substring(0));
					   
					   //Print out results of each test
					   System.out.print("Expected: ");
					   System.out.print(valid);
					   System.out.print(" Actual: ");
					   System.out.print(result);
					   System.out.print(" URL: ");
					   System.out.print(testUrl);
					   System.out.println();
	
					   //Empty out the URL string
					   testUrl.delete(0,testUrl.length());
				   }
			   }
		   }
	   }
   }
   
   public static void testIsValidRandom()
   {
	 
	 //Instantiate random number generator
	 Random rand = new Random();
	 
	 //Instantiate url validator
	 UrlValidator urlValidator = new UrlValidator(UrlValidator.ALLOW_LOCAL_URLS);
	 
	 //Establish StringBuilder object for URL generation
	 StringBuilder testUrlSB = new StringBuilder();
	 
	 //Declare Generex Object
	 Generex generex;
	 
	 //Run tests...
	 for(int i = 0, sabotageMask = 0; i < RANDOM_TEST_RUNS; i++) {
		 testUrlSB.setLength(0);
		 
		 //Randomly pick a valid scheme from the defaults
		 int randomScheme = rand.nextInt(3);
		 if(randomScheme == 0) testUrlSB.append("http://");
		 else if(randomScheme == 1) testUrlSB.append("https://");
		 else testUrlSB.append("ftp://");
		 
		 //If sabotage mask indicates, falsify scheme
		 if((sabotageMask & SABOTAGE_SCHEME) > 0) {
			 for(int j = 0, randomChar; j < testUrlSB.length() - 3; j++) {
				 do{
					 randomChar = 48 + rand.nextInt(75);
				 }while(((randomChar >= 58 && randomChar <= 64) ||
						(randomChar >= 91 && randomChar <= 96)) ||
						 randomChar == testUrlSB.charAt(j));
				 testUrlSB.replace(j, j+1, Character.toString((char)randomChar));
			 }
		 }
		 
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
			 
			 //If sabotage mask indicates, falsify authority
			 if((sabotageMask & SABOTAGE_AUTHORITY) > 0) {
				 //'?' and '#' are excluded here because UrlValidator will immediately consider
				 //any part of the string thereafter as part of the 'query' or 'fragment, 
				 //respectively. (This may be considered a bug, because a URL can be 
				 //validated even if it has no proper domain! Note however that an empty 
				 //authority is allowed with a scheme of 'file' [to account for a file], 
				 //though this circumstance is not tested here.)
				 generex = new Generex("([^?#A-Za-z0-9]+[\\-\\.]?)+");
			 }
			 else {
				 generex = new Generex("([A-Za-z0-9]+[\\-\\.]?)+");
			 }
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
			 
			 //Use Generex to append random ip4v address onto testUrlSB... 
			 
			//If sabotage mask indicates, falsify ipv4 address (authority)
			 if((sabotageMask & SABOTAGE_AUTHORITY) > 0) {
				 generex = new Generex("\\-?(2[6-9][0-9]\\.){3}([1-2]?[0-9]?[6-9]){1}");
			 }
			 else {
				 generex = new Generex("([1-2]?[1-5]?[0-5]\\.){3}([1-2]?[1-5]?[0-5]){1}");
			 }
			 String randomAuthority = generex.random();
			 testUrlSB.append(randomAuthority);
			 //System.out.println("Authority: " + randomAuthority);
		 }
		 
		 /* One-Third of the time use IPV6 for authority */
		 else {
			//Use Generex to append random ipv6 address onto testUrlSB...
			
			//Use compression 50% of the time
			if(rand.nextInt()% 2 == 0)
					
					//No compression...
					
					//If sabotage mask indicates, falsify ipv6 address (authority)
					if((sabotageMask & SABOTAGE_AUTHORITY) > 0) {
						generex = new Generex("\\[([0-9a-zA-Z]{3}[g-zG-Z]:){7}[0-9a-fA-Z]{4}\\]");
					}
					else {
						generex = new Generex("\\[([0-9a-fA-F]{4}:){7}[0-9a-fA-F]{4}\\]");
					}
			else {
				
				//Compression...
				
				//Randomly determine number of hextexts 
				//(which indicates number of hextexts compressed)
				int numCompHextets = 1 + rand.nextInt(7);
				
				if(numCompHextets == 7) {
					if(rand.nextInt() % 2 == 0)
						//If sabotage mask indicates, falsify ipv6 address (authority)
						if((sabotageMask & SABOTAGE_AUTHORITY) > 0) {
							generex = new Generex("\\[::[0-9a-zA-Z]{3}[g-zG-Z]\\]");
						}
						else{
							generex = new Generex("\\[::[0-9a-fA-F]{4}\\]");
						}
					else
						//If sabotage mask indicates, falsify ipv6 address (authority)
						if((sabotageMask & SABOTAGE_AUTHORITY) > 0) {
							generex = new Generex("\\[[0-9a-zA-Z]{3}[g-zG-Z]::\\]");
						}
						else {
						generex = new Generex("\\[[0-9a-fA-F]{4}::\\]");
						}
				}
				else {
					//Compression position starts at 1 (max is 6)
					int compPos = rand.nextInt(7 - numCompHextets) + 1;
					//If sabotage mask indicates, falsify ipv6 address (authority)
					if((sabotageMask & SABOTAGE_AUTHORITY) > 0) {
						generex = new Generex("\\[([0-9a-zA-Z]{3}[g-zG-Z]:){"+String.valueOf(compPos)+"}" +
										  ":([0-9a-zA-Z]{3}[g-zG-Z]:){"+String.valueOf(7-numCompHextets-compPos)+"}" +
										  "[0-9a-zA-Z]{3}[g-zG-Z]\\]");
					}
					else {
						generex = new Generex("\\[([0-9a-fA-F]{4}:){"+String.valueOf(compPos)+"}" +
								  ":([0-9a-fA-F]{4}:){"+String.valueOf(7-numCompHextets-compPos)+"}" +
								  "[0-9a-fA-F]{4}\\]");
					}
					
				}
			}
			String randomAuthority = generex.random();
			testUrlSB.append(randomAuthority);
			//System.out.println("Authority: " + randomAuthority);
		 }
			 
		 //50% chance we use a port number, 
		 //unless port is  sabotaged, in which case port is always used.
		 if(rand.nextInt() % 2 == 0 || ((sabotageMask & SABOTAGE_PORT) > 0)) {
			 testUrlSB.append(":");
			//If sabotage mask indicates, falsify port number
			 if((sabotageMask & SABOTAGE_PORT) > 0) {
				 if(rand.nextInt() % 2 == 0) {
					 testUrlSB.append("-");
				 }
				 else {
					 testUrlSB.append(String.valueOf(rand.nextInt(65536) + 65536));
				 }
			 }
			 testUrlSB.append(String.valueOf(rand.nextInt(65536)));
		 }
		 
		 
		 //75% chance we use a path,
		 //unless path is sabotaged, in which case path always is used.
		 if((1 + rand.nextInt(4)) % 3 != 0 || ((sabotageMask & SABOTAGE_PATH) > 0)) {
			 //Use Generex to randomly generate a path string based on PATH_REGEX 
			 //(see UrlValidator.java line 167). Max length for random path is 80.
			 
			//If sabotage mask indicates, falsify path
			 if((sabotageMask & SABOTAGE_PATH) > 0) {
				//'?' and '#' are excluded here because UrlValidator will immediately consider
				 //any part of the string thereafter as part of the 'query' or 'fragment, 
				 //respectively.
				 generex = 
						 new Generex("(/[^?#\\-A-Za-z0-9:@&=+,!*'$_;\\(\\).%])+");
			 }
			 else{
				 generex = new Generex("(/[-A-Za-z0-9:@&=+,!*'$_;\\(\\)]+(%[A-Fa-f0-9]{2})?(\\.)?)+");
			 }
			 String randomPath = generex.random(rand.nextInt(81));		//80 char max
			 //May still exceed 80 characters based on last pattern match, so truncate if necessary
			 if(randomPath.length() > 80) randomPath = randomPath.substring(0, 81);
			 testUrlSB.append(randomPath);
			 //System.out.println("Path: " + randomPath);
		 }
		 
		 //50% chance we use a query,
		 //unless query is sabotaged, in which case query is always used.
		 if(rand.nextInt() % 2 == 0 || ((sabotageMask & SABOTAGE_QUERY) > 0)) {
			 //Use Generex to randomly generate a path string based on PATH_REGEX 
			 //(see UrlValidator.java line 167). Max length for random path is 80.
			 
			 //If sabotage mask indicates, falsify query
			 if((sabotageMask & SABOTAGE_QUERY) > 0) {
				 generex = new Generex("?\\s+=\\s+(&\\s+=\\s+)*");
			 }
			 else {
				 generex = new Generex("?\\w+=\\w+(&\\w+=\\w+)*");
			 }
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
		 if(sabotageMask == 0 && urlValidator.isValid(testUrl) == false) {
			 System.out.println(String.valueOf(i) + "    " + "false (should be true)" + "    " + testUrl);
		 }
		 else if(sabotageMask > 0 && urlValidator.isValid(testUrl) == true) {
			 System.out.println(String.valueOf(i) + "    " + "true (should be false)" + "    " + testUrl);
			 System.out.println("Sabotage Mask: " + sabotageMask);
		 }
		 
		 //JUnit configuration... only reports first failure, then exits
//		 if(sabotageMask == 0) assertTrue(String.valueOf(i) + "    " + 
//				 "false (should be true)" + "    " + testUrl, urlValidator.isValid(testUrl));
//		 else assertFalse(String.valueOf(i) + "    " + " "
//		 		+ "	true (should be false)" + "    " + testUrl, urlValidator.isValid(testUrl));
		 
		 //Activate and cycle sabotageMask for the second-to-last one-quarter of tests
		 if(i >= RANDOM_TEST_RUNS * 0.5 && i < RANDOM_TEST_RUNS * 0.75 &&
				 sabotageMask < SABOTAGE_SCHEME +
				                SABOTAGE_AUTHORITY +
				 				SABOTAGE_PORT +
				 				SABOTAGE_PATH +
				 				SABOTAGE_QUERY) {
			 sabotageMask++;
		 }
		 else if(i >= RANDOM_TEST_RUNS * 0.5 && i < RANDOM_TEST_RUNS * 0.75) {
			 sabotageMask = 1;
		 }
		 //Randomize mask for last one-quarter of tests
		 else if(i >= RANDOM_TEST_RUNS * 0.75) {
			 sabotageMask = 1 + rand.nextInt(31);
		 }
	 } 
   }
   
   public static void testIsValid()
   {
	   
	   //Unit Tests
	   testIsValidUnit();
	   
	   //Random Tester
	   testIsValidRandom();
   }
	
   public static void main(String[] args) {
	   
	   UrlValidatorTest.testIsValid();
	   
   }

}

// References
// https://stackoverflow.com/questions/4669692/valid-characters-for-directory-part-of-a-url-for-short-links
// https://docs.oracle.com/javase/7/docs/api/java/util/regex/Pattern.html
// https://github.com/mifmif/Generex
// https://github.com/cs-au-dk/dk.brics.automaton
// http://junit.sourceforge.net/javadoc/org/junit/Assert.html#assertFalse(java.lang.String,%20boolean)
// https://stackoverflow.com/questions/5192512/how-can-i-clear-or-empty-a-stringbuilder
// https://stackoverflow.com/questions/8499698/trim-a-string-based-on-the-string-length/8499776
// https://softwareengineering.stackexchange.com/questions/311413/how-should-we-represent-an-ipv6-address-with-port-number-in-text
// https://community.cisco.com/t5/ipv6/what-do-you-call-the-ipv6-address-parts/td-p/1690102
// https://help.eclipse.org/neon/index.jsp?topic=%2Forg.eclipse.jdt.doc.user%2Ftasks%2Ftask-set_method_breakpoints.htm
