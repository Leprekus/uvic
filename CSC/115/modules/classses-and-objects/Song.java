// Song.java

public class Song {
	// 1. generalize what you use to describe a song?
	//    these dictate the fields/attributes in our Song class
	String title;
    String artist;
	private String secret;

	public Song(String title, String artist) {
        this.title = title;
        this.artist = artist;
		
    }

	private Song() {
		secret = "my very well kept secret";
	}

	private static void main(String[] args) {
		Song privateSong = new Song();
		System.out.println(privateSong.getSecret());
	}
	// 2. create fields in Song class

	// 3. go to tester and create some song objects...

	// 4. change fields to private in Song class

	String getSongTitle() {
		return this.title;
	}

	String getArtist() {
		return this.artist;
	}

	String setSongTitle(String newTitle) {
		this.title = newTitle;
		return this.title;
	}

	String setSongArtist(String newArtitst) {
		this.artist = newArtitst;
		return this.artist;
	}

	public String getSecret() {
		return secret;
	}

	// 5. add getters and setters for fields

	// 6. how do we initialize a song with data upon creation?
	//  - add a custom constructor

	// 7. add a toString method
	//  - takes no arguments and returns a String
	//    representing this object
   
}