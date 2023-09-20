public class A2Tester {
	private static int testPassCount = 0;
	private static int testCount = 0;
	private static double THRESHOLD = 0.01;
	
	public static void main(String[] args) {
		
		// Part 1
		testBikeConstructor();
		// testToString();
		// testEquals();
		// testIsLocatedIn();
		// testHasInPortfolio();
		// testTotalSales();
	
		// Part 2
		// testMoreSoldUnits();
		// testMostSoldBike();
		// testMostSoldBikeFromBrand();
		// testTotalBikesSoldFromBrand();
		// testBrandWithHighestSales();
		
		System.out.println("PASSED " + testPassCount + " / " + testCount + " tests");
	}
	
	
	public static void testBikeConstructor() {
		System.out.println("\nTesting Bike constructor...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4a = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike4b = new Bike("Allez Sport", "mountain", 9000, 2499.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		String actualModel;
		String actualType;
		int actualSoldUnits;
		double actualUnitPrice;
		String expectedModel;
		String expectedType;
		int expectedSoldUnits;
		double expectedUnitPrice;
		
		// testing bike2
		expectedModel = new String("Contend AR");
		expectedType = new String("road");
		expectedSoldUnits = 9000;
		expectedUnitPrice = 1449.00;

		actualModel = bike2.getModel();
		//System.out.println("\nExpected: "+expectedModel+". Result returned: "+actualModel);
		displayResults(actualModel.equals(expectedModel), "Bike constructor model set: " + expectedModel);
		
		actualType = bike2.getType();
		//System.out.println("\nExpected: "+expectedType+". Result returned: "+actualType);
		displayResults(actualType.equals(expectedType), "Bike constructor type set: " + expectedType);
		
		actualSoldUnits = bike2.getSoldUnits();
		//System.out.println("\nExpected: "+actualSoldUnits+". Result returned: "+expectedSoldUnits);
		displayResults(actualSoldUnits==expectedSoldUnits, "Bike constructor soldUnits set: " + expectedSoldUnits);
		
		actualUnitPrice = bike2.getUnitPrice();
		//System.out.println("\nExpected: "+actualUnitPrice+". Result returned: "+expectedUnitPrice);
		displayResults(Math.abs(actualUnitPrice-expectedUnitPrice) < THRESHOLD, "Bike constructor price set: " + expectedUnitPrice);
		

		// testing bike6

		expectedModel = new String("Turbo Vado SL");
		expectedType = new String("city");
		expectedSoldUnits = 3000;
		expectedUnitPrice = 4999.00;

		actualModel = bike6.getModel();
		displayResults(actualModel.equals(expectedModel), "Bike constructor model set: " + expectedModel);
		
		actualType = bike6.getType();
		displayResults(actualType.equals(expectedType), "Bike constructor type set: " + expectedType);
		
		actualSoldUnits = bike6.getSoldUnits();
		displayResults(actualSoldUnits==expectedSoldUnits, "Bike constructor soldUnits set: " + expectedSoldUnits);
		
		actualUnitPrice = bike6.getUnitPrice();
		displayResults(Math.abs(actualUnitPrice-expectedUnitPrice) < THRESHOLD, "Bike constructor price set: " + expectedUnitPrice);


		// add tests until you are confident your implementation works correctly
		

	}
	
	public static void testToString() {
		System.out.println("\nTesting Bike toString()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4a = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike4b = new Bike("Allez Sport", "mountain", 9000, 2499.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		String result;
		String expected;
		
		expected = new String("Propel Advanced SL(road)");
		result = bike1.toString();
		displayResults(result.equals(expected), "bike1 toString: " + result);

		expected = new String("Contend AR(road)");
		result = bike2.toString();
		displayResults(result.equals(expected), "bike2 toString: " + result);

		expected = new String("Allez Sport(mountain)");
		result = bike4a.toString();
		displayResults(result.equals(expected), "bike4a toString: " + result);

		// add tests until you are confident your implementation works correctly

	}

	public static void testEquals() {
		System.out.println("\nTesting Bike equals()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4a = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike4b = new Bike("Allez Sport", "mountain", 9000, 2499.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		boolean result;
		
		result = bike1.equals(bike2);
		displayResults(result==false, "bike1 equals bike2: " + result);
		result = bike1.equals(bike3);
		displayResults(result==false, "bike1 equals bike3: " + result);
		result = bike2.equals(bike3);
		displayResults(result==false, "bike2 equals bike3: " + result);
		result = bike1.equals(bike4a);
		displayResults(result==false, "bike1 equals bike4a: " + result);
		result = bike2.equals(bike5);
		displayResults(result==false, "bike2 equals bike5: " + result);
		result = bike4a.equals(bike4b);
		displayResults(result==true, "bike4a equals bike4b: " + result);
		result = bike4a.equals(bike5);
		displayResults(result==false, "bike4a equals bike5: " + result);

		// add tests until you are confident your implementation works correctly

	}
	
	public static void testIsLocatedIn() {
		System.out.println("\nTesting Brand isLocatedIn()...");
		Brand brand1 = new Brand("Giant Bicicles", null, "TW");
		Brand brand2 = new Brand("Specialized", null, "US");
		Brand brand3 = new Brand("Norco", null, "CA");
		
		boolean result;
		boolean expected;
		
		expected = false;
		result = brand1.isLocatedIn("US");
		displayResults(result==expected, "brand1 isLocatedIn: expected " + expected +", returned "+ result);

		expected = true;
		result = brand1.isLocatedIn("TW");
		displayResults(result==expected, "brand1 isLocatedIn: expected " + expected +", returned "+ result);

		expected = true;
		result = brand2.isLocatedIn("US");
		displayResults(result==expected, "brand2 isLocatedIn: expected " + expected +", returned "+ result);

		// add tests until you are confident your implementation works correctly

	}

		
	public static void testHasInPortfolio() {
		System.out.println("\nTesting hasInPortfolio()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4 = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		Bike[] portfolio1 = {bike1, bike2, bike3};
		Bike[] portfolio2 = {bike4, bike5, bike6};
		Bike[] portfolio3 = {bike7, bike8};
		
		Brand brand1 = new Brand("Giant Bicicles", portfolio1, "TW");
		Brand brand2 = new Brand("Specialized", portfolio2, "US");
		Brand brand3 = new Brand("Norco", portfolio3, "CA");
		

		boolean result;
		boolean expected;
		
		result = brand1.hasInPortfolio(bike1);
		expected = true;
		displayResults(result==expected, "brand1 hasInPortfolio bike1: expected " + expected +", returned "+ result);

		result = brand1.hasInPortfolio(bike3);
		expected = true;
		displayResults(result==expected, "brand1 hasInPortfolio bike3: expected " + expected +", returned "+ result);
		
		result = brand1.hasInPortfolio(bike5);
		expected = false;
		displayResults(result==expected, "brand1 hasInPortfolio bike5: expected " + expected +", returned "+ result);
		
		// add tests until you are confident your implementation works correctly
		
	}


	public static void testTotalSales() {
		System.out.println("\nTesting totalSales()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4 = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		Bike[] portfolio1 = {bike1, bike2, bike3};
		Bike[] portfolio1a = {bike1, bike2};
		Bike[] portfolio2 = {bike4, bike5, bike6};
		Bike[] portfolio3 = {bike7, bike8};
		
		Brand brand1 = new Brand("Giant Bicicles", portfolio1, "TW");
		Brand brand2 = new Brand("Specialized", portfolio2, "US");
		Brand brand3 = new Brand("Norco", portfolio3, "CA");
		
		double result;
		double expected;
		
		result = brand1.totalSales();
		expected = 1700*10999.00 + 9000*1449.00 + 500*5699.00;
		displayResults(Math.abs(result-expected)<THRESHOLD, "brand1 totalSales: expected " + expected +", returned "+ result);
	
		brand1 =  new Brand("Giant Bicicles", portfolio1a, "TW");
		result = brand1.totalSales();
		expected = 1700*10999.00 + 9000*1449.00;
		displayResults(Math.abs(result-expected)<THRESHOLD, "brand1 totalSales: expected " + expected +", returned "+ result);
	
		result = brand3.totalSales();
		expected = 4000*4449.00 + 30000*899.00;
		displayResults(Math.abs(result-expected)<THRESHOLD, "brand1 totalSales: expected " + expected +", returned "+ result);
	
		// add tests until you are confident your implementation works correctly
	
	}
	
	
	public static void testMoreSoldUnits() {
		System.out.println("\nTesting moreSoldUnits()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4 = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		Bike[] portfolio1 = {bike1, bike2, bike3};
		Bike[] portfolio1a = {bike1, bike2};
		Bike[] portfolio2 = {bike4, bike5, bike6};
		Bike[] portfolio3 = {bike7, bike8};
		
		Brand brand1 = new Brand("Giant Bicicles", portfolio1, "TW");
		Brand brand2 = new Brand("Specialized", portfolio2, "US");
		Brand brand3 = new Brand("Norco", portfolio3, "CA");
		
		int result;
		int expected;
		
		result = A2Exercises.moreSoldUnits(bike1, bike2);
		expected = 9000;
		displayResults(result==expected, "moreSoldUnits bike1 vs. bike2: expected " + expected +", returned "+ result);
		
		result = A2Exercises.moreSoldUnits(bike4, bike7);
		expected = 15000;
		displayResults(result==expected, "moreSoldUnits bike4 vs. bike7: expected " + expected +", returned "+ result);
		
		// add tests until you are confident your implementation works correctly
		
	}
	
	
	public static void testMostSoldBike() {
		System.out.println("\nTesting mostSoldBike()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4 = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		Bike[] array1 = {bike1};
		Bike[] array2 = {bike1, bike4, bike7};
		Bike[] array3 = {bike1, bike4, bike7, bike8};
		Bike[] array4 = {bike1, bike2, bike3, bike4, bike5, bike6, bike7, bike8};
		
		Bike result;
		Bike expected;
		
		result = A2Exercises.mostSoldBike(array1);
		expected = bike1;
		displayResults(result==expected, "mostSoldBike bike1: expected " + expected +", returned "+ result);

		result = A2Exercises.mostSoldBike(array2);
		expected = bike4;
		displayResults(result==expected, "mostSoldBike bike1, bike4, bike7: expected " + expected +", returned "+ result);

		// add tests until you are confident your implementation works correctly
		
	}
	
	
	public static void testMostSoldBikeFromBrand() {
		System.out.println("\nTesting mostSoldBikeFromBrand()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4 = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		Bike[] portfolio1 = {bike1, bike2, bike3};
		Bike[] portfolio1a = {bike1, bike3};
		Bike[] portfolio2 = {bike4, bike5, bike6};
		Bike[] portfolio3 = {bike7, bike8};
		
		Brand brand1 = new Brand("Giant Bicicles", portfolio1, "TW");
		Brand brand2 = new Brand("Specialized", portfolio2, "US");
		Brand brand3 = new Brand("Norco", portfolio3, "CA");
		
		Bike result;
		Bike expected;
		
		result = A2Exercises.mostSoldBikeFromBrand(brand1);
		expected = bike2;
		displayResults(result==expected, "mostSoldBikeFromBrand brand1: expected " + expected +", returned "+ result);

		result = A2Exercises.mostSoldBikeFromBrand(brand2);
		expected = bike4;
		displayResults(result==expected, "mostSoldBikeFromBrand brand2: expected " + expected +", returned "+ result);

		// add tests until you are confident your implementation works correctly
		
	}
	
		
	public static void testTotalBikesSoldFromBrand() {
		System.out.println("\nTesting totalBikesSoldFromBrand()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4 = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		Bike[] portfolio1 = {bike1, bike2, bike3};
		Bike[] portfolio1a = {bike1, bike3};
		Bike[] portfolio2 = {bike4, bike5, bike6};
		Bike[] portfolio3 = {bike7, bike8};
		
		Brand brand1 = new Brand("Giant Bicicles", portfolio1, "TW");
		Brand brand2 = new Brand("Specialized", portfolio2, "US");
		Brand brand3 = new Brand("Norco", portfolio3, "CA");
		
		int result;
		int expected;
		
		result = A2Exercises.totalBikesSoldFromBrand(brand1);
		expected = 1700+9000+500;
		displayResults(result==expected, "totalBikesSoldFromBrand brand1: expected " + expected + ", returned " + result);

		// add tests until you are confident your implementation works correctly
		
	}
	
	
	public static void testBrandWithHighestSales() {
		System.out.println("\nTesting brandWithHighestSales()...");
		Bike bike1 = new Bike("Propel Advanced SL", "road", 1700, 10999.00);
		Bike bike2 = new Bike("Contend AR", "road", 9000, 1449.00);
		Bike bike3 = new Bike("Anthem Advanced Pro 29", "mountain", 500, 5699.00);
		Bike bike4 = new Bike("Allez Sport", "mountain", 15000, 1449.00);
		Bike bike5 = new Bike("Allez Sport", "road", 1000, 3099.00);
		Bike bike6 = new Bike("Turbo Vado SL", "city", 3000, 4999.00);
		Bike bike7 = new Bike("Aurum A1", "road", 4000, 4449.00);
		Bike bike8 = new Bike("Scene 2", "city", 30000, 899.00);
		
		Bike[] portfolio1 = {bike1, bike2, bike3};
		Bike[] portfolio1a = {bike1, bike2};
		Bike[] portfolio2 = {bike4, bike5, bike6};
		Bike[] portfolio3 = {bike7, bike8};
		
		Brand brand1 = new Brand("Giant Bicicles", portfolio1, "TW");
		Brand brand2 = new Brand("Specialized", portfolio2, "US");
		Brand brand3 = new Brand("Norco", portfolio3, "CA");
		
		Brand[] array1 = {brand1};
		Brand[] array2 = {brand1, brand3};
		Brand[] array3 = {brand2, brand3};
		Brand[] array4 = {brand1, brand2, brand3};

		Brand result;
		Brand expected;
		
		result = A2Exercises.brandWithHighestSales(array1);
		expected = brand1;
		displayResults(result==expected, "brandWithHighestSales brand1: expected " + expected +", returned "+ result);

		result = A2Exercises.brandWithHighestSales(array3);
		expected = brand3;
		displayResults(result==expected, "brandWithHighestSales brand2, brand3: expected " + expected +", returned "+ result);
	
		// add tests until you are confident your implementation works correctly

	}

	public static void displayResults (boolean passed, String testName) {
		testCount++;
		if (passed) {
			System.out.println ("Passed test: " + testName);
			testPassCount++;
		} else {
			System.out.println ("Failed test: " + testName + " at line "
								+ Thread.currentThread().getStackTrace()[2].getLineNumber());
		}
	}
}
	