import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

public class BlockGroup extends Entity {
    public boolean oriented;
    public boolean sub;

    public TreeSet<Pair> pairs;
    public int n;

    public Pair bottomleft;
    public Pair topright;
    public Pair boundingbox;

    public BlockGroup(boolean orientation, boolean subtractive, Pair[] v, Color c) {
        oriented = orientation;
        sub = subtractive;
        pairs = new TreeSet<Pair>();
        n = v.length;
        color = c;

        bottomleft = new Pair(Integer.MAX_VALUE, Integer.MAX_VALUE);
        topright = new Pair(Integer.MIN_VALUE, Integer.MIN_VALUE);

        for (int i = 0; i < n; i++) {
            pairs.add(v[i]);
            bottomleft.x = Math.min(bottomleft.x, v[i].x);
            bottomleft.y = Math.min(bottomleft.y, v[i].y);
            topright.x = Math.max(topright.x, v[i].x);
            topright.y = Math.max(topright.y, v[i].y);
        }

        boundingbox = new Pair(topright.x - bottomleft.x + 1, topright.y - bottomleft.y + 1);
    }

    public BlockGroup(boolean orientation, boolean subtractive, Pair[] v) {
        this(orientation, subtractive, v, Color.YELLOW);
    }

    public void updateBounds() {
        bottomleft = new Pair(Integer.MAX_VALUE, Integer.MAX_VALUE);
        topright = new Pair(Integer.MIN_VALUE, Integer.MIN_VALUE);

        for (Pair p : pairs) {
            bottomleft.x = Math.min(bottomleft.x, p.x);
            bottomleft.y = Math.min(bottomleft.y, p.y);
            topright.x = Math.max(topright.x, p.x);
            topright.y = Math.max(topright.y, p.y);
        }

        boundingbox = new Pair(topright.x - bottomleft.x + 1, topright.y - bottomleft.y + 1);
    }

    // Utility Functions

    public boolean contains(Pair p) {
        return pairs.contains(p);
    }

    public void add(Pair p) {
        if (contains(p)) return;
        pairs.add(p.clone());
        n++;
    }

    public void remove(Pair p) {
        if (!contains(p)) return;
        pairs.remove(p);
        n--;
    }

    public void reset(Pair[] v) {
        n = v.length;
        pairs.clear();
        bottomleft = new Pair(Integer.MAX_VALUE, Integer.MAX_VALUE);
        topright = new Pair(Integer.MIN_VALUE, Integer.MIN_VALUE);

        for (int i = 0; i < n; i++) {
            pairs.add(v[i]);
            bottomleft.x = Math.min(bottomleft.x, v[i].x);
            bottomleft.y = Math.min(bottomleft.y, v[i].y);
            topright.x = Math.max(topright.x, v[i].x);
            topright.y = Math.max(topright.y, v[i].y);
        }

        boundingbox = new Pair(topright.x - bottomleft.x + 1, topright.y - bottomleft.y + 1);
    }

    public BlockGroup clone() {
        Pair[] v = new Pair[pairs.size()];
        int index = 0;
        for (Pair p : pairs) v[index++] = p.clone();
        return new BlockGroup(this.oriented, this.sub, v, this.color);
    }

    // General Functions

    public void rotate(int x) {
        while (x < 0) x += 8;
        x %= 4;
        if (x <= 0) return;
        rotate(x - 1);

        Pair[] res = new Pair[pairs.size()];
        int index = 0;
        for (Pair p : pairs) res[index++] = new Pair(-1 * p.y, p.x);
        reset(res);
    }

    public void move(Pair n) {
        Pair[] res = new Pair[pairs.size()];
        int index = 0;
        for (Pair p : pairs) res[index++] = p.add(n);
        reset(res);
    }

    public void invmov(Pair n) {
        move(n.inv());
    }

    public void normalize() {
        invmov(bottomleft);
    }

    public void removeRegion(BlockGroup o) {
        for (Pair p : o.pairs) this.remove(p);
    }

    public void addRegion(BlockGroup o) {
        for (Pair p : o.pairs) this.add(p);
    }

    public String toString() {
        StringBuilder sb = new StringBuilder("[O = ");
        sb.append(this.oriented ? "1" : "0");
        sb.append(" S = ");
        sb.append(this.sub ? "1" : "0");
        sb.append(" N = ");
        sb.append(Integer.toString(n));
        sb.append("] = [");
        for (Pair i : pairs) sb.append(i.toString());
        sb.append("]");
        return sb.toString();
    }

    // RegionTesting

    public boolean containsBB(BlockGroup b) { // Can the bounding box contain that belonging to region b?
        if (boundingbox.x < b.boundingbox.x) return false;
        if (boundingbox.y < b.boundingbox.y) return false;
        return true;
    }

    public boolean directoverlay(BlockGroup b) { // Does this region contain region b in terms of absolute coordinates?
        if (n < b.n) return false;
        for (Pair p : b.pairs) {
            if (!contains(p)) return false;
        }
        return true;
    }

    ArrayList<Pair> fixedoverlay(BlockGroup b) { // Does this region contain region b? Return the offset if yes, INT_MIN if no.
        ArrayList<Pair> res = new ArrayList<Pair>();
        if (n < b.n) return res;

        int dx = bottomleft.x - b.bottomleft.x;
        int dy = bottomleft.y - b.bottomleft.y;

        BlockGroup test = b.clone();
        test.move(new Pair(dx, dy));

        int width = Math.abs(boundingbox.x - b.boundingbox.x);
        int height = Math.abs(boundingbox.y - b.boundingbox.y);

        Pair carriagereturn = new Pair(1, -1 * height - 1);

        for (int i = 0; i <= width; i++) {
            for (int j = 0; j <= height; j++) {
                if (directoverlay(test)) res.add(new Pair(i, j));
                test.move(Pair.PLUS_Y);
            }
            test.move(carriagereturn);
        }

        return res;
    }

    ArrayList<ArrayList<Pair>> overlay(BlockGroup b) {
        ArrayList<ArrayList<Pair>> res = new ArrayList<ArrayList<Pair>>();
        res.add(fixedoverlay(b));
        if (!b.oriented) {
            BlockGroup test = b.clone();
            test.normalize();
            for (int i = 0; i < 3; i++) {
                test.rotate(1);
                test.normalize();
                res.add(fixedoverlay(test));
            }
        }

        return res;
    }

    // Now for the real thing

    public boolean dfsUtil(BlockGroup region, BlockGroup[] v, int index) {
        if (index >= v.length) return region.n == 0;

        BlockGroup group = v[index].clone();
        group.normalize();
        ArrayList<ArrayList<Pair>> options = region.overlay(group);

        boolean res = false;
        for (int i = 0; i < 4; i++) {
            group.move(region.bottomleft);
            for (Pair op : options.get(i)) {
                group.move(op);
                region.removeRegion(group);
                res |= dfsUtil(region, v, index + 1);
                if (res) return true;
                region.addRegion(group);
                group.invmov(op);
            }
            if (group.oriented) break;
            group.rotate(1);
            group.normalize();
        }
        return res;
    }

    public boolean solve(BlockGroup[] v) {
        int diff = n;
        for (BlockGroup i : v) {
            if (i.sub) diff += i.n;
            else diff -= i.n;
        }
        if (diff != 0) return false;
        return dfsUtil(clone(), v, 0);
    }
}
