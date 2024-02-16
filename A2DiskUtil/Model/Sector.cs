namespace A2DiskUtil.Model
{
    public class Sector
    {
        private byte[] sectorData = new byte[256];

        public Sector(byte[] sourceData, int offset)
        {
            Array.Copy(sourceData, offset, sectorData, 0, sectorData.Length);
        }

        public byte[] Read(int offset, int length)
        {
            byte[] result = new byte[length];
            Array.Copy(sectorData, offset, result, 0, length);
            return result;
        }

        public byte ReadByte(int offset)
        {
            return Read(offset, 1)[0];
        }
    }
}