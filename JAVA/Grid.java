import javax.xml.stream.events.StartDocument;
import java.awt.*;
import java.util.*;

public class Grid {
    public int n;
    public int m;
    Entity[][] board;
    TreeSet<Pair> starts;
    TreeSet<Pair> ends;
    TreeSet<Pair> dots;
    TreeSet<Pair> triangles;
    TreeSet<Pair> blobs;
    TreeSet<Pair> stars;
    TreeSet<Pair> blocks;
    TreeSet<Pair> cancels;
    TreeSet<Pair> ignored;

    Pair begin;

    public Grid(Entity[][] v) {
        n = v.length;
        m = 0;
        for (Entity[] i : v) m = Math.max(i.length, m);
        if (n % 2 == 0) n++;
        if (m % 2 == 0) m++;

        board = new Entity[n][m];

        starts = new TreeSet<Pair>();
        ends = new TreeSet<Pair>();
        dots = new TreeSet<Pair>();
        triangles = new TreeSet<Pair>();
        blobs = new TreeSet<Pair>();
        stars = new TreeSet<Pair>();
        blocks = new TreeSet<Pair>();
        cancels = new TreeSet<Pair>();
        ignored = new TreeSet<Pair>();

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (j < v[i].length) board[i][j] = v[i][j];
                if (Util.isStartingPoint(board[i][j])) {
                    begin = new Pair(i, j);
                    starts.add(begin.clone());
                }
                Entity e = board[i][j];
                Pair p = new Pair(i, j);
                if (Util.isEndingPoint(e)) ends.add(p);
                if (e instanceof Dot) dots.add(p);
                if (e instanceof Star) stars.add(p);
                if (e instanceof Triangle) triangles.add(p);
                if (e instanceof Blob) blobs.add(p);
                if (e instanceof BlockGroup) blocks.add(p);
                if (e instanceof Cancel) cancels.add(p);
            }
        }
    }

    public void defaultGrid() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if ((i % 2 == 0) || (j % 2 == 0)) board[i][j].isPath = true;
            }
        }
    }

    void drawLine(Pair a, Pair b) {
        if (a.x == b.x) {
            for (int i = Math.min(a.y, b.y); i <= Math.max(a.y, b.y); i++) board[a.x][i].isPathOccupied = true;
        }
        else if (a.y == b.y) {
            for (int i = Math.min(a.x, b.x); i <= Math.max(a.x, b.x); i++) board[i][a.y].isPathOccupied = true;
        }
    }

    void drawPath(Pair[] v) {
        if (v.length < 2) return;
        for (int i = 1; i < v.length; i++) drawLine(v[i - 1], v[i]);
    }

    public String toString() {
        StringBuilder s = new StringBuilder("");
        for (Entity[] i : board) {
            for (Entity j : i) {
                char open = j.isPathOccupied ? '[' : (j.isPath ? '+' : '_');
                char close = j.isPathOccupied ? ']' : (j.isPath ? '+' : '_');
                s.append(open);
                s.append(Util.getType(j));
                s.append(close);
                s.append(" ");
            }
            s.append("\n");
        }
        s.substring(0, s.length() - 1);
        return s.toString();
    }

    // Verification

    public boolean inside(Pair p) {
        if (p.x < 0 || p.y < 0) return false;
        if (p.x >= n || p.y >= m) return false;
        return true;
    }

    public final int DX[] = {01, 00, -1, 00};
    public final int DY[] = {00, 01, 00, -1};
    public final Pair DZ[] = {Pair.PLUS_X, Pair.PLUS_Y, Pair.MINUS_X, Pair.MINUS_Y};

    public boolean ver(int sx, int sy) {
        if (!Util.isStartingPoint(board[sx][sy])) return false;
        Entity e = board[sx][sy];
        if (!e.isPathOccupied) return false;

        TreeSet<Pair> vis = new TreeSet<Pair>();
        Queue<Pair> q = new LinkedList<Pair>();
        q.add(new Pair(sx, sy));

        boolean reachedend = false;

        while (q.size() > 0) {
            Pair p = q.poll();
            vis.add(p);

            for (int i = 0; i < 4; i++) {
                Pair next = p.add(DZ[i]);
                if (!inside(next)) continue;
                Entity n = board[next.x][next.y];
                if (Util.isEndingPoint(n)) reachedend = true;
                if (!n.isPath || !n.isPathOccupied) continue;
                if (vis.contains(next)) continue;
                vis.add(next);
                q.add(next);
            }
        }

        if (!reachedend) return false;

        TreeSet<Pair> violations = new TreeSet<Pair>();

        // DOTS + TRIANGLES

        for (Pair i : dots) {
            Entity o = board[i.x][i.y];
            if (!o.isPathOccupied) violations.add(i);
        }

        for (Pair i : triangles) {
            Entity o = board[i.x][i.y];
            if (!(o instanceof Triangle)) continue;
            Triangle ox = (Triangle)o;
            int target = ox.x;
            int count = 0;
            for (int ii = 0; ii < 4; ii++) {
                Pair p = i.add(DZ[ii]);
                if (!inside(p)) continue;
                Entity o2 = board[p.x][p.y];
                if (o2.isPath && o2.isPathOccupied) count++;
            }
            if (count != target) violations.add(i);
        }

        // BLOBS + STARS

        HashMap<Color, ArrayList<Entity>> ding = new HashMap<>();
        HashMap<Color, Integer> selectedcolors = new HashMap<>();
        TreeSet<Pair> collected = new TreeSet<>();

        vis.clear();

        for (Pair ii : blobs) {
            if (vis.contains(ii)) continue;
            ding.clear();
            selectedcolors.clear();
            collected.clear();
            q.clear();
            q.add(ii);

            while (q.size() > 0) {
                Pair now = q.poll();
                vis.add(now);

                Entity cur = board[now.x][now.y];
                if (cur instanceof Blob) {
                    if (!ding.containsKey(cur.color)) ding.put(cur.color, new ArrayList<Entity>());
                    (ding.get(cur.color)).add(cur);

                    collected.add(now);
                    if (!selectedcolors.containsKey(cur.color)) selectedcolors.put(cur.color, 0);
                    selectedcolors.put(cur.color, 1 + selectedcolors.get(cur.color));
                }

                for (int i = 0; i < 4; i++) {
                    Pair mid = now.add(DZ[i]);
                    Pair next = mid.add(DZ[i]);

                    if (!inside(mid) || !inside(next)) continue;
                    Entity between = board[mid.x][mid.y];
                    if (between.isPathOccupied) continue;
                    if (vis.contains(next)) continue;
                    vis.add(next);
                    q.add(next);
                }
            }

            Color maxColor = null;
            int maxfreq = -1;
            for (Color i : selectedcolors.keySet()) {
                if (!ding.containsKey(i)) continue;
                int truefreq = ding.get(i).size();
                if (truefreq > maxfreq) {
                    maxfreq = truefreq;
                    maxColor = i;
                }
            }

            boolean hasmorecolors = false;
            for (Color i : ding.keySet()) {
                if (i != null && i != maxColor) hasmorecolors = true;
            }

            for (Pair c : collected) {
                Entity e2 = board[c.x][c.y];
                if (e2.color != null && e2.color != maxColor) violations.add(c);
                else if (hasmorecolors) violations.add(c);
            }
        }

        vis.clear();

        for (Pair ii : stars) {
            if (vis.contains(ii)) continue;
            ding.clear();
            selectedcolors.clear();
            collected.clear();
            q.clear();
            q.add(ii);

            while (q.size() > 0) {
                Pair now = q.poll();
                vis.add(now);

                Entity cur = board[now.x][now.y];
                if (!ding.containsKey(cur.color)) ding.put(cur.color, new ArrayList<Entity>());
                (ding.get(cur.color)).add(cur);
                if (cur instanceof Star) {

                    collected.add(now);
                    if (!selectedcolors.containsKey(cur.color)) selectedcolors.put(cur.color, 0);
                    selectedcolors.put(cur.color, 1 + selectedcolors.get(cur.color));
                }

                for (int i = 0; i < 4; i++) {
                    Pair mid = now.add(DZ[i]);
                    Pair next = mid.add(DZ[i]);

                    if (!inside(mid) || !inside(next)) continue;
                    Entity between = board[mid.x][mid.y];
                    if (between.isPathOccupied) continue;
                    if (vis.contains(next)) continue;
                    vis.add(next);
                    q.add(next);
                }
            }

            for (Pair i : collected) {
                Entity e2 = board[i.x][i.y];
                Color c = e2.color;
                if (!ding.containsKey(c)) {
                    violations.add(i);
                    continue;
                }
                if (ding.get(c).size() != 2) violations.add(i);
            }
        }

        // BLOCK GROUPS

        vis.clear();
        TreeSet<Pair> region = new TreeSet<Pair>();

        for (Pair ii : blocks) {
            if (vis.contains(ii)) continue;
            region.clear();
            collected.clear();
            q.clear();
            q.add(ii);

            while (q.size() > 0) {
                Pair now = q.poll();
                vis.add(now);
                region.add(now);

                Entity cur = board[now.x][now.y];
                if (cur instanceof BlockGroup) collected.add(now);

                for (int i = 0; i < 4; i++) {
                    Pair mid = now.add(DZ[i]);
                    Pair next = mid.add(DZ[i]);

                    if (!inside(mid) || !inside(next)) continue;
                    Entity between = board[mid.x][mid.y];
                    if (between.isPathOccupied) continue;
                    if (vis.contains(next)) continue;
                    vis.add(next);
                    q.add(next);
                }
            }

            Pair regionvec[] = new Pair[region.size()];
            int index = 0;
            for (Pair i : region) regionvec[index++] = new Pair(i.y / 2, -1 * (i.x / 2));
            BlockGroup testregion = new BlockGroup(true, false, regionvec);

            BlockGroup pieces[] = new BlockGroup[collected.size()];
            index = 0;
            for (Pair p : collected) {
                Entity o = board[p.x][p.y];
                if (!(o instanceof BlockGroup)) continue;
                pieces[index++] = (BlockGroup)(o);
            }
            if (testregion.solve(pieces)) {}
            else {
                for (Pair p : collected) violations.add(p);
            }
        }

        // CANCELS

        if (cancels.size() == 0 || ignored.size() == cancels.size()) return violations.size() == 0;

        if (violations.size() == 0) return false;

        vis.clear();
        for (Pair ii : cancels) {
            if (ignored.contains(ii)) continue;
            if (vis.contains(ii)) continue;
            collected.clear();
            q.clear();
            q.add(ii);

            while (q.size() > 0) {
                Pair now = q.poll();
                vis.add(now);
                region.add(now);

                Entity cur = board[now.x][now.y];
                if (Util.isSymbol(cur) && !(cur instanceof Cancel)) {
                    if (violations.contains(cur)) collected.add(now);
                }

                for (int i = 0; i < 4; i++) {
                    Pair next = now.add(DZ[i]);

                    if (!inside(next) || !inside(next)) continue;
                    Entity between = board[next.x][next.y];
                    if (between.isPathOccupied) continue;
                    if (vis.contains(next)) continue;
                    vis.add(next);
                    q.add(next);
                }
            }

            ignored.add(ii);
            ((Cancel)(board[ii.x][ii.y])).ignored = true;

            boolean retval = false;
            for (Pair p : collected) {
                Entity o = board[p.x][p.y];
                board[p.x][p.y] = o.clear();
                if (o instanceof Dot) dots.remove(p);
                else if (o instanceof Blob) blobs.remove(p);
                else if (o instanceof Star) stars.remove(p);
                else if (o instanceof Triangle) triangles.remove(p);
                else if (o instanceof BlockGroup) blocks.remove(p);

                if (ver(sx, sy)) retval = true;

                board[p.x][p.y] = o;
                if (o instanceof Dot) dots.add(p);
                else if (o instanceof Blob) blobs.add(p);
                else if (o instanceof Star) stars.add(p);
                else if (o instanceof Triangle) triangles.add(p);
                else if (o instanceof BlockGroup) blocks.add(p);
                if (retval) break;
            }

            ignored.remove(ii);
            ((Cancel)(board[ii.x][ii.y])).ignored = false;

            if (retval) return true;
        }


        // End

        return false;
    }

    public boolean check() {
        return ver(begin.x, begin.y);
    }
}
