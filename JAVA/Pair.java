public class Pair implements Comparable<Pair> {
    public int x;
    public int y;

    public Pair() {
        x = 0;
        y = 0;
    }

    public Pair(int a, int b) {
        x = a;
        y = b;
    }

    public Pair clone() {
        return new Pair(this.x, this.y);
    }

    public int compareTo(Pair o) {
        if (x != o.x) return x - o.x;
        return y - o.y;
    }

    public Pair add(Pair other) {
        return new Pair(this.x + other.x, this.y + other.y);
    }

    public Pair inv() {
        return new Pair(-1 * this.x, -1 * this.y);
    }

    public Pair sub(Pair other) {
        return this.add(other.inv());
    }

    public boolean equals(Pair o) {
        return this.compareTo(o) == 0;
    }

    public String toString() {
        return "<" + Integer.toString(this.x) + " " + Integer.toString(this.y) + ">";
    }



    public static final Pair PLUS_X = new Pair(1, 0);
    public static final Pair PLUS_Y = new Pair(0, 1);
    public static final Pair MINUS_X = new Pair(-1, 0);
    public static final Pair MINUS_Y = new Pair(0, -1);
}
