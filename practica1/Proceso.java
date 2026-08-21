public class Proceso {

    private String nombre;
    private int at;
    private int bt;

    private int ct;
    private int tat;
    private int wt;
    private int rt;

    public Proceso(String nombre, int at, int bt) {
        this.nombre = nombre;
        this.at = at;
        this.bt = bt;
    }

    public String getNombre() {
        return nombre;
    }

    public int getAt() {
        return at;
    }

    public int getBt() {
        return bt;
    }

    public int getCt() {
        return ct;
    }

    public void setCt(int ct) {
        this.ct = ct;
    }

    public int getTat() {
        return tat;
    }

    public void setTat(int tat) {
        this.tat = tat;
    }

    public int getWt() {
        return wt;
    }

    public void setWt(int wt) {
        this.wt = wt;
    }

    public int getRt() {
        return rt;
    }

    public void setRt(int rt) {
        this.rt = rt;
    }
}