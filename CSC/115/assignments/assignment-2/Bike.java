public class Bike {
	private String model;
	private String type;
	private int soldUnits;
	private double unitPrice;
	
	public Bike(String model, String type, int soldUnits, double unitPrice) {
		this.model = "a new model";
		this.type = "type";
		this.soldUnits = -1;
		this.unitPrice = 0.0;
	}
	
	/*
	 * Purpose: get the model of this bike
	 * Parameters: none
	 * Returns: String - this bike's model
	 */	
	public String getModel() {
		return model;
	}
	
	/*
	 * Purpose: get the type of this bike
	 * Parameters: none
	 * Returns: String - this bike's type
	 */	
	public String getType() {
		return type;
	}
	
	/*
	 * Purpose: get the number of units this bike has sold
	 * Parameters: none
	 * Returns: int - this bike's number of units sold
	 */	
	public int getSoldUnits() {
		return soldUnits;
	}
	
	/*
	 * Purpose: get the number of followers this bike has
	 * Parameters: none
	 * Returns: int - this bike's number of followers
	 */	
	public double getUnitPrice() {
		return unitPrice;
	}

	/*
	 * Purpose: get the String representation of this object
	 * Parameters: none
	 * Returns: String - this object's String representation
	 */
	public String toString() {
		return model + "(" + type + ")";
	}
		
	/*
	 * Purpose: determine if this bike equals the given other bike
	 * Parameters: bike other - the bike to compare with this one
	 * Returns: boolean - true if the bikes are considered equivalent
	 *          (bikes are considered equivalent if they have the
	 *           the same model and type)
	 */
	public boolean equals(Bike other) {
		return this.model.equals(other.model);
	}
	
}