public class Brand {
	private String name;
	private Bike[] portfolio;
	private String country; // country acronym
	
	public Brand(String title, Bike[] portfolio, String country) {
		this.name = title;
	    this.portfolio = portfolio;
        this.country = country;
	}

	/*
	 * Purpose: get the name of this brand
	 * Parameters: none
	 * Returns: String - this brand's name
	 */
	public String getName() {
		return name;
	}
	
	/*
	 * Purpose: get this brand's portfolio of bikes
	 * Parameters: none
	 * Returns: Bike[] - this brands's portfolio
	 */
	public Bike[] getPortfolio() {
		return portfolio;
	}
	
	/*
	 * Purpose: get the brand's country of origin
	 * Parameters: none
	 * Returns: String - this brand's country of origin
	 */
	public String getCountry() {
		return country;
	}

	/*
	 * Purpose: get the String representation of this object
	 * Parameters: none
	 * Returns: String - this object's String representation
	 */
	public String toString() {
		return this.name + "(" + country + ")";
	}

	/*
	 * Purpose: determine if this brand equals the given other brand
	 * Parameters: brand other - the brand to compare with this one
	 * Returns: boolean - true if the brands are considered equivalent
	 *          (brands are considered equivalent if they have the
	 *           the same name and country)
	 */
	public boolean equals(Brand other) {
		return this.name.equals(other.name)
			&& this.country.equals(other.country);
	}
	
	/*
	 * Purpose: determine if this brand is located in the given country
	 * Parameters: String country - a country acronym to compare to
	 * Returns: boolean - true if this brand is located in the given country
	 */
	public boolean isLocatedIn(String country) {
		return this.country.equals(country);
	}

	/*
	 * Purpose: get the total sales, in dollars, of this brand's bike models
	 * Parameters: none
	 * Returns: double - the total sales in dollars of this brand's bike models
	 */
	public double totalSales() {
		double sum = 0.0;
		
		return sum; // so it compiles
	}

	/*
	 * Purpose: determine whether the given bike is one of the bikes
	 * in this brand's portfolio
	 * Parameters: bike alias - the bike in question
	 * Returns: boolean - true if this given bike is in the brand's portfolio
	 */
	public boolean hasInPortfolio(Bike alias) {
		boolean found = false;
		
		return found; // so it compiles
	}
}
