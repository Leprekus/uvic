/*
 * ClubLine
 * 
 * A class to simulate a lineup of clubbers waiting to enter a
 * dance club. Some clubbers come alone and go to the back of 
 * the line, others (VIPs) bribe their way to a certain position. 
 * Sometimes someone buys a bunch of tickets at once so 
 * multiple clubbers can be removed from the line and enter
 * the club at the same time.
 */
public class ClubLine {
	Queue<Clubber> lineup;
	//Do not add any other fields

	public ClubLine() {
		//Do not change the constructor in any way
		lineup = new GenericQueue<Clubber>();
	}
	
	/*
	 * Purpose: adds a clubber to line to enter the club
	 * Parameter: Clubber p - the clubber to add
	 * Returns void - nothing
	 */
	public void enterLine(Clubber cl) {
		// TODO: implement this
		lineup.enqueue(cl);
	}
	
	/*
	 * Purpose: gets the number of clubbers in line
	 * Parameters: none
	 * Returns: int - number of clubbers in line
	 */
	public int clubbersInLine() {
		// TODO: implement this
		
		return lineup.size(); // so it compiles
	}
	
	/*
	 * Purpose: lets the first clubber in line enter the club
	            (allows them to purchase their ticket
				 and enter the dance club)
	 * Parameter: none
	 * Returns: Clubber - the clubber who gets their ticket
	 *                   and is no longer waiting in line
	 */
	public Clubber nextClubber() {
		// TODO: implement this
		try {
			Clubber popped  = lineup.dequeue();
			return popped;
		} catch(EmptyQueueException e) {
			return null;
		}
		
	}
	
	/*
	 * Purpose: handles a whole group of clubbers waiting in line
	 *          (allow a group to buy a number of tickets 
	 *           and all of them enter the dance club)
	 * Parameters: int num - the number of clubbers
	 * Returns int - the number of clubbers who were successfully
	 *               able to be removed from the line
	 */
	public int nextGroup(int num) {
		// TODO: implement this
		int size = lineup.size();
		if(num >= size) {
			lineup.dequeueAll();
			return size;
		}
		try {

			int counter = 0;
			while (counter < num){
				lineup.dequeue();
				++counter;
			}
			return num; // so it compiles
		} catch(EmptyQueueException e){
			return 0;
		}
	}

	
	/*
	 * Purpose: accepts a bribe to put someone into a specific
	 *          position in the line to get into the dance club
	 * Parameters: Clubber vip - the clubber entering the line
	 *             int pos - the position they are trying to get to
	 * Returns boolean - true if person added to line, false otherwise
	 */
	public boolean nextVIP(Clubber vip, int pos) {
		// TODO: implement this
		if(pos > lineup.size() || pos < 0) return false;
		if(pos == lineup.size()){
			lineup.enqueue(vip);
			return true;
		}
		try {
			int len = lineup.size();
			Clubber[] items = new Clubber [ len ];
			for(int i = 0; i < len; ++i) {
				Clubber item = lineup.dequeue();
				items[i] = item;
			}
			//get array in lifo correct order
			for(int i = 0, counter = 0; i < len; ++i, ++counter) {
				if(counter == pos) {
					lineup.enqueue(vip);
				}
				
				Clubber item = items[i];
				lineup.enqueue(item);
				
			}
			return true;

		} catch(EmptyQueueException e) {
			System.out.println(e.getStackTrace());
			return false;
		}
	}
	
}
	
	