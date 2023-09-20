public class VehicleTesterSoln {

	public static void main(String[] args) {
		
		MotorCycle m = new MotorCycle("CB300R");
		System.out.println(m.getKMs() + "kms traveled");
		
		/* Q1 - what will be output after the following
			   call to the drive method? */

		m.drive(10);
		System.out.println(m.getKMs() + "kms traveled");

		System.out.println();

		/* Q2 - implement the AutomaticCar class and then
				uncomment and run the tests below */

		AutomaticCarSoln a1 = new AutomaticCarSoln("Mazda 3", 67000);
		System.out.println(a1.getKMs());
		a1.drive(50);
		System.out.println(a1.getKMs());
		
		System.out.println("trying again after buckling up");
		a1.fastenSeatbelts();
		a1.drive(50);
		System.out.println(a1.getKMs());

		System.out.println();
		System.out.println("** Starting Q3 **\n");
		/* Q3 - Make an array of vehicles and drive 
			  each vehicle in the array 120km */
			  
		Vehicle mcycle = new MotorCycle("Harley Davidson");
		Vehicle car = new AutomaticCarSoln("Mustang", 15000);

		Vehicle[] collection = {m, a1, mcycle, car};
		for (int i = 0; i < collection.length; i++) {
			if (collection[i] instanceof MotorCycle) {
				System.out.println("This vehicle is a motorcycle");
			} else if (collection[i] instanceof AutomaticCarSoln) {
				System.out.println("This is a car");
				AutomaticCarSoln temp = (AutomaticCarSoln) collection[i];
				temp.fastenSeatbelts();
				//((AutomaticCarSoln) collection[i]).fastenSeatbelts();
			}	
			
			System.out.println("Let's go for a drive!");
			collection[i].drive(120);
			System.out.println("this car has now driven "+collection[i].getKMs()+ "KMs");
			System.out.println();
		}
	}

}
