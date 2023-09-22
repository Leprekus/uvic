public class Bike {
	private String model;
	private String type;
	private int soldUnits;
	private double unitPrice;
	
	public Bike(String model, String type, int soldUnits, double unitPrice) {
		// this.model = model;
		// this.type = type;
		// this.soldUnits = soldUnits;
		// this.unitPrice = unitPrice;
		setModel( model );
		setType( type );
		setSoldUnits( soldUnits );
		setUnitPrice( unitPrice );

	}
	public void setModel(String model) {
		if(model.length() > 0) {
			this.model = model;
		} else {
			throw new IllegalArgumentException("Model's name must be greater than 0");
		}
	}
	public void setType(String type) {
		if(type.length()  > 0){
			this.type = type;
		} else {
			throw new IllegalArgumentException("Type's length must be greater than 0");
		}
	}
	public void setSoldUnits(int soldUnits) {
		if(soldUnits >= 0) {
			this.soldUnits = soldUnits;
		} else {
			throw new IllegalArgumentException("Sold units cannot be less than 0");
		}
	}
	public void setUnitPrice(double unitPrice) {
		if(unitPrice >= 0) {
			this.unitPrice = unitPrice;
		} else {
			throw new IllegalArgumentException("Unit price cannot be less than 0");
		}
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
		boolean isModel =  this.getModel().equals(other.getModel());
		boolean isSoldUnits =  this.getSoldUnits() == other.getSoldUnits();
		boolean isType =  this.getType().equals(other.getType());
		boolean isUnitPrice =  this.getUnitPrice() == other.getUnitPrice();
		boolean isEqual = isModel && isSoldUnits && isType && isUnitPrice;
		
		return isEqual;
	}
	
}