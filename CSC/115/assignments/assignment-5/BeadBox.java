public class BeadBox {
    private int numberBeads;
    private double weightPerBead;
    
    public BeadBox(int numberBeads, double weightPerBead) {
        this.numberBeads = numberBeads;
        this.weightPerBead = weightPerBead;
    }

    public int getNumberBeads() {
        return numberBeads;
    }

    public double getWeightPerBead() {
        return weightPerBead;
    }

    public double boxWeight() {
        return numberBeads * weightPerBead;
    }

    public String toString() {
        return "(" + numberBeads + ", " + weightPerBead + ")";
    }
}
