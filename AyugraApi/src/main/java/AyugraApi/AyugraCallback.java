package AyugraApi;

public interface AyugraCallback {
    void onReceivedPacket(String packet);
    void onSentPacket(String packet);
}
