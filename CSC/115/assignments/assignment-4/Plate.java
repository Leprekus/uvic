public class Plate {	
	private int diameter;
	
	public Plate (int diameter) {
		this.diameter = diameter;
	}
	
	public int getDiameter() {
		return diameter;
	}
	
	public String toString() {
		return Integer.toString(diameter);
	}
}