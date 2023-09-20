public class AutomaticCarSoln implements Vehicle {
	private String model;
	private int distanceTraveled;
	private boolean seatbeltsOn;

	public AutomaticCarSoln(String model, int distanceTraveled){
		this.model = model;
		this.distanceTraveled = distanceTraveled;
		seatbeltsOn = false;
	}

	public void fastenSeatbelts() {
		seatbeltsOn = true;
	}

	public boolean safetyCheck() {
		if (seatbeltsOn) {
			return true;
		} else {
			return false;
		}
	}

	public void drive(int distance) {
		if (safetyCheck()) {
			System.out.println("Driving "+distance+" kms");
			this.distanceTraveled += distance;
		} else {
			System.out.println("It is not safe to drive");
		}
	}

	public int getKMs() {
		return distanceTraveled;
	}

	public static void main (String[] args) {
		AutomaticCar a1 = new AutomaticCar("Mazda 3", 67000);
		System.out.println(a1.getKMs());
		a1.buckleSeatbelts();
		a1.drive(50);
		System.out.println(a1.getKMs());
	}
}
