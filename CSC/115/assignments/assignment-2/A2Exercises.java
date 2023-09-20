public class A2Exercises {

	/*
	 * Purpose: get the total of units sold of the more sold bike
	 * Parameters: Bike bike1, Bike bike2
	 * Returns: int - the total of units sold of the more sold bike
	 */
	public static int moreSoldUnits(Bike bike1, Bike bike2) {
		return 0; // so it compiles
	}
	
	/*
	 * Purpose: get the Bike with the most units sold
	 * Parameters: Bike[] array - the array of bikes
	 * Returns: Bike - the bike with the most units sold
	 *
	 * Note: if two bikes have the same number of units sold, 
	 *       the one who comes first in the array is returned
	 *       (ie. the element with lowest index number)
	 *
	 * Preconditions: The array is not empty
	 */
	public static Bike mostSoldBike(Bike[] array) {
		Bike most = array[0];
				
		return most; // so it compiles
	}
	
	/*
	 * Purpose: get the Bike with the most units sold from a brand
	 * Parameters: Brand brand - the brand in question
	 * Returns: Bike - the bike with the most units sold from that brand
	 *
	 * Note: if two bikes from the brand have the same number of units 
	 *       sold, the one who comes first in the array is returned
	 *       (ie. the element with lowest index number)
	 *
	 * Preconditions: The brand has at least one bike in its portfolio
	 */
	public static Bike mostSoldBikeFromBrand(Brand brand) {
		Bike[] array = brand.getPortfolio();

		return array[0]; // so it compiles
	}

	/*
	 * Purpose: get a count of the number of sold bikes
	 *          from a brand
	 * Parameters: Brand brand - the brand in question
	 * Returns: int - the number of bikes sold in the array 
	 *                of Bikes in the brand's portfolio
	 */	
	public static int totalBikesSoldFromBrand(Brand brand) {
		int count = 0;
		
		return count;  // so it compiles
	}
	
	/*
	 * Purpose: get the brand in the given array
	 *          who has the highest sales in currency value
	 * Parameters: Brand[] array - the array of brands to compare
	 * Returns: Brand - the brand with highest values in sales
	 *                    in currency
	 *
	 * Note: if two brands have the same sales, the brand
	 *       that comes first in the array is returned 
	 *        (i.e. with the lowest index number)
	 *
	 * Preconditions: The array is not empty
	 */
	public static Brand brandWithHighestSales(Brand[] array) {
		Brand highestSalingBrand = array[0];
		
		return highestSalingBrand; // so it compiles
	}
	
}