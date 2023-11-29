import java.awt.*;

public class Example { // EXAMPLE

    public static void main(String[] args) {
        Pair[] bar3 = {new Pair(0, 1), new Pair(0, 2), new Pair(0, 3)};
        Pair[] bar2 = {new Pair(0, 0), new Pair(0, 1)};
        Pair[] lshape = {new Pair(100, 100), new Pair(100, 101), new Pair(101, 100)};
        Pair[] threebythree = new Pair[9];
        Pair[] cell = {new Pair(1000, 1000)};
        for (int i = 0; i < 9; i++) threebythree[i] = new Pair(i % 3, i / 3);

        BlockGroup workbench = new BlockGroup(true, false, threebythree);
        BlockGroup corner = new BlockGroup(false, false, lshape);
        BlockGroup spline = new BlockGroup(true, false, bar3);
        BlockGroup unit = new BlockGroup(true, false, cell);

        BlockGroup[] v = {corner, corner, spline};
        System.out.println(workbench.solve(v));

        Entity[][] plapp = {{new Entity(), new Entity(), new Endpoint(false)}, {new Entity(), corner, new Entity()}, {new Endpoint(true), new Entity(), new Entity()}};
        Grid grid = new Grid(plapp);
        grid.defaultGrid();
        System.out.println(grid);

        grid.drawLine(new Pair(2, 0), new Pair(0, 0));
        grid.drawLine(new Pair(0, 0), new Pair(0, 2));
        System.out.println(grid);

        System.out.println(grid.check());
    }
}
