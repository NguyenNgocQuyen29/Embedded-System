## LESSION 1. GPIO ##
### Để cấu hình cho ngoại vi GPIO ta follow theo các bước sau: ###
  ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/8b64ddcf-fd3c-4214-b78c-0e90d7be0214)
     
 - **Cấp xung clock cho ngoại vi**: ta nhìn vào diagram của con vi điều khiển ta thấy sử dụng API được cấp sẵn cho từng bus, các ngoại vi trên bus được cấp xung thông qua việc truyền thông số vào các API.
     
      ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/011fe33b-f1ee-42d7-83c3-b939150eb278)

    - Ví dụ: nếu bạn muốn sử dụng chân PA2 thì bạn phải cấp xung cho GPIO, muốn cấp xung cho GPIO thì phải cấp xung cho bus mà GPIO treo lên là APB2.
    - Trong VDK STM32 thì thanh ghi dùng để cấp clock là thanh ghi **RCC**
    - Mình sử dụng thư viện chuẩn, cú pháp để cấp/không cấp clock cho một ngoại vi là: **RCC_@PeriphClockCmd(A, B)** , với @ là tên bus mà mình muốn cấp clock(AHB, APB1, APB2), A là ngoại vi mà mình muốn cấp
 clock(RCC_APB2Periph_GPIOC,....), B là cho phép cấp/không cấp (ENABLE,...).
- **Cấu hình GPIO**: GPIO có các tham số đó là pin, mode, speed.
  - Pin: là chân mà bạn muốn dùng( 0->15)
  - Mode: loại chức năng mà bạn muốn dùng, 2 chức năng cơ bản nhất của GPIO chính là Input và Output. Cú pháp để dùng: **GPIO_Mode_Out_PP** //push-
    
    👀👀Input:(đầu vào) có nhiều loại ví dụ như **floating, input_pullup, input_pulldown**
       - floating thì mình cũng chưa hiểu rõ nữa hì hì.
       - input_pullup: dùng điện trở kéo lên nguồn, trạng thái mặc định là 1.
       - input_pulldown: dùng điện trở kéo xuống đất, trạng thái mặc định là 0.
         
    *Để hiểu rõ hơn thì nôn na là: ví dụ bạn có 1 nút nhấn đi, bạn để trạng thái mặc định là 1(tức là chưa làm gì thì trạng thái nó là 1), khi tác động trạng thái nó là 0. Lúc đó bạn thiết kế bài toán
là nhấn nút led sẽ sáng thì làm sao nó biết được là bạn nhấn nút? thì lúc đó nếu nó nhận được tín hiệu 0(tức là có sự thay đổi trạng thái từ 0 về 1) thì nó sẽ thực hiện lệnh bật đèn.*

    👀👀Output: ở ví dụ trên thì nút nhấn là input và led là output, output có 2 loại đó là opendrain và push-pull(pull-up & pull-down).
       - open drain: mn muốn hiểu rõ thì search gg để xem nha còn mình cứ hiểu nôn na là nó có 2 trạng thái là 0 và mức trở kháng cao *(không phải là 1)*
         
       - push pull: cũng có 2 chế độ **pull-up & pull-down** giống ở phía trên.
         
    *Khi nào dùng opendrain và khi nào dùng push-pull? Khi bạn muốn on/off,pwm thì dùng push. Còn opendrain chưa dùng nên chưa biết , nào biết update sau 😊*

  - Speed: tốc độ đáp ứng thì tùy mình thôi.
    
  **Vấn đề đặt ra ở đây là? trong một chương trình mình muốn dùng 1 lúc nhiều chân đi chẳng lẻ lần nào mình cũng gọi các tham số ra , như thế sẽ rất chi là mệt, nên để tối ưu thì mình sẽ tạo 1 struct mà nó
    có cùng tính chất như:mode, pin, speed. Trong thư viện chuẩn, struct đó có tên là** GPIO_InitTypeDef. Struct là một kiểu dữ liệu nên mình sẽ khai báo 1 biến và dùng biến đó trỏ tới các tham số trong struct. Các cấu
    hình sẽ lưu giá trị vào biến đó.
  
  Vậy là đã cấu hình cho 1 chân đã xong, và mình sẽ truyền 2 tham số đó là **tên GPIO**:GPIO cần dùng và **cấu hình mình vừa mới config**: con trỏ tới biến đó.
  
## LESSION 2. CÁC CHUẨN GIAO TIẾP CƠ BẢN ##  
### 1.Truyền nhận dữ liệu ##
Việc truyền nhận dữ liệu thực chất là việc truyền nhận các tín hiệu điện áp biểu diễn cho các bit, điện áp được truyền nhận trên các chân của Vi điều khiển. 

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/835ae92d-4018-4b5d-a82c-ed16c9e2bf43)
**Câu hỏi được đặt ra ở đây là :ví dụ 2 bit liền kề nó có cùng mức điện áp thì làm sao MCU nhận biết được đó là 2 bit?** => Đó là lí do tại sao có các kiểu giao tiếp sau đây.
### 2.SPI - Serial Peripheral Interface ##
SPI 
  >- Là chuẩn giao tiếp nối tiếp(các bit sẽ được truyền đi lần lượt) đồng bộ.
  >- Hoạt động ở chế độ song công(ở cùng thời điểm có thể vừa truyền vừa nhận).
  >- Sử dụng 4 dây.

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/6fcc1cb1-7746-4101-94ba-497b05fcc2a8)

>- SCK: Serial Clock. Thiết bị tạo clock là master và cung cấp clock cho slave. Sở dỉ là giao tiếp đồng bộ vì kiểu master và slave có chung 1 dây SCK để điều khiển truyền hay nhận của 2 thiết bị.
>- MOSI: Master Out Slave In . Tín hiệu tạo bởi master và Slave nhận tín hiệu.
>- MISO: Master In Slave Out. Tín hiệu tạo bởi Slave và Master nhận tín hiệu
>- SS: Select Slave. Chọn thiết bị Slave để giao tiếp. Để chọn được thì *Master kéo đường SS tương ứng xuống mức 0(bình thường SS ở mức 1)*.

Quá trình truyền nhận SPI:
 ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/d4ed44ed-206e-4517-93eb-ea22abf7cb8d)

- Việc truyền data thì nó cũng sẽ đi từng bước lần lượt. Mỗi bit đi nó sẽ cấp 1 xung clock(xung clock được cấp bằng cách kéo chân SCK lên 1 rồi về 0, bình thường trạng thái SCK là 0).

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/bafed7bc-b5ab-4869-968b-a773cd95aaa6)


    - Bắt đầu truyền nhận master sẽ kéo chân CS của slave xuống 0 để báo hiệu quá trình truyền nhận.
    - Clock sẽ được cấp bởi master ,mỗi xung clock thì Master sẽ truyền 1 bit cho slaveslave và slave cũng truyền 1bit cho master.
    - Các thanh ghi cập nhật giá trị truyền nhận và dịch 1 bit.
    - truyền cho tới khi hết 8 bit.
    - Giao tiếp song công(một lúc có thể truyền nhận).

Trạng thái các xung Clock được xác định dựa vào CPOL và CPHA"
- CPOL:
   + Bằng 0 thì lúc mặc định là 0. Muốn tạo ra clock để báo hiệu truyền nhận thì ta kéo nó từ 0 lên 1 rồi về 0 là tạo ra 1 xung clock để truyền nhận 1 bit
   + Bằng 1 thì lúc mặc định là 1. Muốn tạo ra clock để báo hiệu truyềnn nhận thì ta kéo nó từ 1 xuống 0 rồi về 1 là tạo ra 1 xung clock để truyền nhận 1 bit.
- CPHA:
   + Bằng 0: tức là mình đưa bit vào trước mới cấp clock.
   + Bằng 1: tức là mình cấp clock rồi mới đưa bit vào.

### 3.I2C - Inter-Integrated Circuit ##
I2C:
![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/d6784c65-8f3b-4f47-adcc-47d9da5e31e0)

  >- Là chuẩn giao tiếp nối tiếp.
  >- Hoạt động ở chế độ bán song công(tức là tại một thời điểm thì nó chỉ có thể truyền hoặc nhận, nếu muốn nhận phải đợi truyền xong).
  >- Sử dụng 2 dây: SCL, SDA, 2 dây này được gắn vào điện trở kéo lên nguồn.

***Bán song công vì: nó chỉ có 2 dây 1 là SCL(clock) và 1 dây là SDA để truyền, chỉ có 1 dây nên không thể 1 lúc mà vừa truyền vừa nhận dữ liệu được.****
I2C nó không truyền theo từng bit giống như SPI mà nó sẽ truyền theo từng frame

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/a13d7528-9d93-4dbd-8c29-2b970a5d46a6)

  >- Đầu tiên phải có start condition( SDA kéo xuống mức 0 trước SCL để tạo ra tín hiệu, bình thường 2 dây này ở mức 1 tại vì nó được gắn vào điện trở kéo lên nguồn).
  >- Tiếp theo là 7 or 10 bit địa chỉ(tùy thuộc vào chip) và ***1 bit R/W***.
  >- Phải gửi địa chỉ vì nó cùng 1 lúc truyền nhận nhiều thiết bị, để phân biệt chúng thì mỗi con phải có 1 địa chỉ, khi nó truyền bit địa chỉ thì tất cả sẽ được nhận m, thiết bị nào ứng với địa chỉ đó thì sẽ biết là sắp có quá trình truyền/nhận. Còn bit R/W để nó nói với slave tương ứng với địa chỉ đó là nó sẽ truyền hay đọc dữ liệu(0:Read là master sẽ đọc được trạng thái của Slave, 1.Write:master viết data cho slave).
 
Ví dụ:
![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/81902655-4b05-4d0c-8f14-77b57f254d18)

Ở ví dụ này Master truyền đi *0b10101111* tức là 7 bit địa chỉ và 1 bit W(1). Khi truyèn đến tất cẩ slave, nó readSDA ghi vào 1 thanh ghi, lấy thanh ghi đó >> 1(bỏ biến R/W ra) sau đó ^ với address của mỗi con
Nếu kết quả là 0 thì tức là địa chỉ của slave đó và thực hiện việc truyền data. (chú ý: 2 số giống nhau ^ sẽ bằng 0, còn khác nhau thì bằng 1).

  >- Bit cuối cùng là ACK để slave báo hiệu cho Master biết là slave đã nhận tín hiệu(báo hiệu bằng cách kéo đường SDA xuống ).

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/df9f5768-cf91-4a9e-ad48-14924c5a729e)

Nếu mà Slave đã nhận data rồi thì phải truyền lại 1 bit gọi là ACK(=0) bit này truyền lên SDA, lúc này Master thành input để đọc giá trị slave gửi tới(bit ACK), nếu mà Master đọc giá trị của SDA là 1 thì là truyền thất bại, phải tryền cái khác hoặc gửi lại. Tóm lại là mỗi lần truyền 8bit thì Master đổi chức năng thành input để đọc xem slave đã nhận được hay chưa.

*Sau khi thực hiện xong quá trình truyền nhận thì kết thúc phải có **stop condition** SCL kéo lên 1 trước*

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/aa2d8985-ea32-4e7a-9154-3e4defbd5f23)


**Note: Giao tiếp đồng bộ là giữa các thiết bị truyền nhận có xung clock, còn không có thì là giao tiếp bất đồng bộ, UART là một ví dụ**


### 3.UART - Universal Asynchronous Receiver-Transmitter ##
Không giống như SPI,I2C có thể thực hiện một lúc nhiều thiết bị, còn UART chỉ 2 thiết bị với nhau.
UART :
>- Bất đồng bộ.
>- 2 dây TX,RX
>- Hoạt động song công.

Truyền theo frame gồm 8 bit
![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/c1e16ab0-4776-4897-8865-eb6f9cd1dcbc)

- Để bắt đầu truyền nhận khi có Start Bit(kéo TX từ 1->0).
- 5 đến 9 bit dữ liệu.
- Parity bit (bit chẵn lẻ).
   + Bit chẵn lẻ kiểm tra xem dữ liệu nhận đúng hay chưa.
   + Quy luật chẵn: nếu tổng số bit 1 là số chẵn thì bit đó là 0, còn quy luật lẻ là nếu tổng sốt bit lẻ là số chẵn thì thêm số 1.
- 1 hoặc 2 stop bit().






  








   


    
  


