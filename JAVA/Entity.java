import java.awt.*;

public class Entity {
    public Color color;
    public boolean isPath;
    public boolean isPathOccupied;

    public Entity() {
        color = null;
        isPath = false;
        isPathOccupied = false;
    }

    public Entity clear() {
        Entity res = new Entity();
        res.isPath = isPath;
        res.isPathOccupied = isPathOccupied;
        res.color = null;
        return res;
    }


}
