# Nodes

- [x] **cordoba**
  
  - pub: `Float32` to **/amazon** every 100ms

- [x] **lyon**
  
  - sub: `Float32` **/amazon***
  - 
  - pub: `Float32` to **/tigris** on receive **/amazon**

- [x] **freeport**
  
  - pub: `Int64` to **/ganges** every 50ms

- [x] **medellin**
  
  - pub: `Int32` to **/nile** every 10ms

- [x] **portsmouth**
  
  - pub: `String` to **/danube** every 200ms (len 256)

- [x] **hamburg**
  
  - sub: `Float32` **/tigris**
  
  - sub: `Int64` **/ganges**
  
  - sub: `Int32` **/nile**
  
  - sub: `String` **/danube**
  
  - 
  
  - pub: same `String` to **/parana** on receive **/danube**

- [x] **delhi**
  
  - pub: `Image` to **/columbia** every 1000ms

- [x] **taipei**
  
  - sub: `Image` **/columbia**
  - 
  - pub: same `Image` to **/colorado** on receive **/columbia**

- [x] **osaka**
  
  - sub: `Image` **/columbia**
  
  - sub: `Image` **/colorado**
  
  - 
  
  - pub: `PointCloud2` to **/salween** on receive **/colorado**
  
  - pub: `LaserScan` to **/godavari** on receive **/colorado**

- [x] **hebron**
  
  - pub: `Quaternion` to **/chenab** every 100ms

- [x] **kingston**
  
  - pub: `Vector3` to **/yamuna** every 100ms

- [x] **tripoli**
  
  - sub: `LaserScan` **/godavari**
  
  - sub: `Image` **/columbia**
  
  - 
  
  - pub: `PointCloud2` to **/loire** on receive **/godavari**

- [x] **mandalay**
  
  - sub: `String` **/danube**
  
  - sub: `Quaternion` **/chenab**
  
  - sub: `PointCloud2` **/salween**
  
  - sub: `LaserScan` **/godavari**
  
  - sub: `Vector3` **/yamuna**
  
  - sub: `PointCloud2` **/loire**
  
  - 
  
  - pub: `Pose` to **/tagus** every 100ms
  
  - pub: `Image` to **/missouri** every 100ms
  
  - pub: `PointCloud2` to **/brazos** every 100ms

- [x] **ponce**
  
  - sub: `Pose` **/tagus**
  
  - sub: `String` **/danube**
  
  - sub: `Image` **/missouri**
  
  - sub: `PointCloud2` **/brazos**
  
  - sub: `Vector3` **/yamuna**
  
  - sub: `LaserScan` **/godavari**
  
  - sub: `PointCloud2` **/loire**
  
  - sub: `Float32` **/ohio**
  
  - sub: `Float64` **/volga**
  
  - 
  
  - pub: `Twist` to **/congo** on receive **/brazos**
  
  - pub: `TwistWithCovarianceStamped` to **/mekong** on receive **/brazos**

- [x] **geneva**
  
  - sub: `String` **/parana**
  
  - sub: `String` **/danube**
  
  - sub: `Pose` **/tagus**
  
  - sub: `Twist` **/congo**
  
  - 
  
  - pub: `String` to **/arkansas** on receive **/parana**

- [x] **monaco**
  
  - sub: `Twist` **/congo**
  
  - 
  
  - pub: `Float32` to **/ohio** on receive **/congo**

- [x] **rotterdam**
  
  - sub: `TwistWithCovarianceStamped` **/mekong**
  
  - 
  
  - pub: `Vector3Stamped` to **/murray** on receive **/mekong**

- [x] **barcelona**
  
  - sub: `TwistWithCovarianceStamped` **/mekong**
  
  - 
  
  - pub: `WrenchStamped` to **/lena** on receive **/mekong**

- [x] **arequipa**
  
  - sub: `String` **/arkansas**

- [x] **georgetown**
  
  - sub: `Vector3Stamped` **/murray**
  
  - sub: `WrenchStamped` **/lena**
  
  - 
  
  - pub: `Float64` to **/volga** every 50ms
