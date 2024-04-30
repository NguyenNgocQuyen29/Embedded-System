<details><summary>LESSON 1: GPIO</summary>
</p>
  
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
       - floating : tín hiệu trên chân GPIO mặc định ở trạng thái không xác định thường phụ thuộc vào nguồn ở bên 
       - input_pullup: dùng điện trở kéo lên nguồn, trạng thái mặc định là 1.
       - input_pulldown: dùng điện trở kéo xuống đất, trạng thái mặc định là 0.
         
    *Để hiểu rõ hơn thì nôn na là: ví dụ bạn có 1 nút nhấn đi, bạn để trạng thái mặc định là 1(tức là chưa làm gì thì trạng thái nó là 1), khi tác động trạng thái nó là 0. Lúc đó bạn thiết kế bài toán
là nhấn nút led sẽ sáng thì làm sao nó biết được là bạn nhấn nút? thì lúc đó nếu nó nhận được tín hiệu 0(tức là có sự thay đổi trạng thái từ 0 về 1) thì nó sẽ thực hiện lệnh bật đèn.*

    👀👀Output: ở ví dụ trên thì nút nhấn là input và led là output, output có 2 loại đó là opendrain và push-pull(pull-up & pull-down).
       - open drain: Khi sử dụng mode Push-pull thì trong đó nó cấu hình sẵn một điện trở rồi(kéo lên hoặc kéo xuống) nhưng ở I2C nó đã có sẵn điện trở kéo lên rồi, nên dùng OD để không cần điện trở kéo cho vi điều khiển nữa. Tại vì mình đã có điện trở kéo lên rồi giờ config thêm nó sẽ gây xung đột 
         
       - push pull: cũng có 2 chế độ **pull-up & pull-down** giống ở phía trên.
         
    *Khi nào dùng opendrain và khi nào dùng push-pull? Khi bạn muốn on/off,pwm thì dùng push. Còn opendrain thì với những giao thức nào ở trạng thái mặc định của nó đã có trở kéo lên nguồn rồi thì trong vi điều khiển ko cần dùng nữa ví dụ:làm việc với giao thức I2C 😊*

  - Speed: tốc độ đáp ứng thì tùy mình thôi.
    
  **Vấn đề đặt ra ở đây là? trong một chương trình mình muốn dùng 1 lúc nhiều chân đi chẳng lẻ lần nào mình cũng gọi các tham số ra , như thế sẽ rất chi là mệt, nên để tối ưu thì mình sẽ tạo 1 struct mà nó
    có cùng tính chất như:mode, pin, speed. Trong thư viện chuẩn, struct đó có tên là** GPIO_InitTypeDef. Struct là một kiểu dữ liệu nên mình sẽ khai báo 1 biến và dùng biến đó trỏ tới các tham số trong struct. Các cấuhình sẽ lưu giá trị vào biến đó.
Vậy là đã cấu hình cho 1 chân đã xong, và mình sẽ truyền 2 tham số đó là **tên GPIO**:GPIO cần dùng và **cấu hình mình vừa mới config**: con trỏ tới biến đó.

</p>

</details>


<details><summary>LESSON 2: CÁC CHUẨN GIAO TIẾP</summary>
<p>
  
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
*Giao tiếp bằng chân chọn chip*
Quá trình truyền nhận SPI:
 ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/d4ed44ed-206e-4517-93eb-ea22abf7cb8d)

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/bafed7bc-b5ab-4869-968b-a773cd95aaa6)

    - Bắt đầu truyền nhận master sẽ kéo chân CS của slave xuống 0 để báo hiệu quá trình truyền nhận.
    - Clock sẽ được cấp bởi master ,mỗi xung clock thì Master sẽ truyền 1 bit cho slave và slave cũng truyền 1bit cho master.
    - Các thanh ghi cập nhật giá trị truyền nhận và dịch 1 bit.
    - truyền cho tới khi hết 8 bit.

Các mode của SPI: trạng thái các xung Clock được xác định dựa vào CPOL và CPHA"
- CPOL:
   + Bằng 0 thì lúc mặc định là 0. Muốn tạo ra clock để báo hiệu truyền nhận thì ta kéo nó từ 0 lên 1 rồi về 0 là tạo ra 1 xung clock để truyền nhận 1 bit
   + Bằng 1 thì lúc mặc định là 1. Muốn tạo ra clock để báo hiệu truyềnn nhận thì ta kéo nó từ 1 xuống 0 rồi về 1 là tạo ra 1 xung clock để truyền nhận 1 bit.
- CPHA:
   + Bằng 0: tức là mình đưa bit vào trước mới cấp clock.
   + Bằng 1: tức là mình cấp clock rồi mới đưa bit vào.

### 3.I2C - Inter-Integrated Circuit ##
I2C:
![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/d6784c65-8f3b-4f47-adcc-47d9da5e31e0)

  >- Hoạt động ở chế độ bán song công(tức là tại một thời điểm thì nó chỉ có thể truyền hoặc nhận, nếu muốn nhận phải đợi truyền xong).
  >- Sử dụng 2 dây: SCL, SDA, 2 dây này được gắn vào điện trở kéo lên nguồn.
***Bán song công vì: nó chỉ có 2 dây 1 là SCL(clock) và 1 dây là SDA để truyền, chỉ có 1 dây nên không thể 1 lúc mà vừa truyền vừa nhận dữ liệu được.****
- I2C nó không truyền theo từng bit giống như SPI mà nó sẽ truyền theo từng frame
- I2C giao tiếp bằng địa chỉ
Data frame của I2C:

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

  >- Sau khi thực hiện xong quá trình truyền nhận thì kết thúc phải có **stop condition** SCL kéo lên 1 trước.

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/aa2d8985-ea32-4e7a-9154-3e4defbd5f23)


**Note: Giao tiếp đồng bộ là giữa các thiết bị truyền nhận có xung clock, còn không có thì là giao tiếp bất đồng bộ, UART là một ví dụ**


### 3.UART - Universal Asynchronous Receiver-Transmitter ##
Không giống như SPI,I2C có thể thực hiện một lúc nhiều thiết bị, còn UART chỉ 2 thiết bị với nhau(giao tiếp 1 - 1)
Tại vì bất đồng bộ nên để 2 chủ thể có thể biết mà tương tác với nhau thì dựa vào **baudrate**. Khác baudrate vẫn truyền được nhưng dư liệu truyền, nhận sẽ bị sai.
UART :
>- Bất đồng bộ.
>- 2 dây TX,RX
>- Hoạt động song công.

Truyền theo frame gồm 8 bit
![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/c1e16ab0-4776-4897-8865-eb6f9cd1dcbc)

- Để bắt đầu truyền nhận khi có Start Bit(kéo TX từ 1->0).
- 5 đến 9 bit dữ liệu.
- 0 to 1 Parity bit (bit chẵn lẻ).
   + Bit chẵn lẻ kiểm tra xem dữ liệu nhận đúng hay chưa.
   + Quy luật chẵn: nếu tổng số bit 1 là số chẵn thì bit đó là 0, còn quy luật lẻ là nếu tổng sốt bit lẻ là số chẵn thì thêm số 1.
- 1 hoặc 2 stop bit(kéo chân Rx lên 1).

***So sánh UART, SPI, I2C***

![144962711_249915713337702_340587713684986630_n](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/bda29e02-1576-4909-9c86-33fc4b3c336b)

</p>

</details>

<details><summary>LESSON 3: CONFIG PERIPHERAL SOFTWARE & HARDWARE</summary>
</p>

## a. I2C SOFTWARE (GPIO)
Các bước để thực hiện I2C software

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/66ad7dc7-24b9-4da4-8677-72acd0979ef0)

*Hàm truyền*:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/ef56abe0-268d-44cb-a07a-14d6b4c10cee)


        #include "stm32f10x_gpio.h"             // Device:StdPeriph Drivers:GPIO
        #include "stm32f10x_rcc.h"              // Device:StdPeriph Drivers:RCC
        #include "stm32f10x_tim.h"              // Device:StdPeriph Drivers:TIM
        
        #define I2C_SCL    GPIO_Pin_6
        #define I2C_SDA    GPIO_Pin_7
        
        #define I2C_GPIO   GPIOB
        
        
        
        #define WRITE_SDA_0       GPIO_ResetBits(I2C_GPIO,I2C_SDA)
        #define WRITE_SDA_1       GPIO_SetBits(I2C_GPIO,I2C_SDA)
        #define WRITE_SCL_0       GPIO_ResetBits(I2C_GPIO,I2C_SCL)
        #define WRITE_SCL_1       GPIO_SetBits(I2C_GPIO,I2C_SCL)
        #define READ_SDA_VAL      GPIO_ReadInputDataBit(I2C_GPIO,I2C_SDA)
        
        typedef enum{
          NOT_OK = 0,
          OK
        }status;
        typedef enum{
          NACK = 0,
          ACK
        }ACK_Bit;
        void delay_us(uint32_t delay){
          TIM_SetCounter(TIM2,0);
          while(TIM_GetCounter(TIM2)<delay){}
        }
        /*
        * Function: I2C_Config
        * Description: initial state setting for SDA and SCL is 1
        */
        void I2C_Config(){
              WRITE_SDA_1;
              delay_us(1);
              WRITE_SCL_1;
              delay_us(1);
        }
        /*
        * Function: I2C_Start
        * Description: Start condition is that SCL pin is pulled down to 0 before SDA pin. To avoid unexpected situations, set both pins to state 1 before Start Condition
        */
        void I2C_Start(){
              //de phong truong hop co nhieu thiet bi truyen voi nhau , lo co truong hop slave dang truyen 0 len duong SDA thi sao nen de mac dinh la 1 
              WRITE_SDA_1;
              delay_us(3);
              WRITE_SCL_1;
              delay_us(3);
              WRITE_SDA_0;
              delay_us(3);
              WRITE_SCL_0;
              delay_us(3);
        }
        void I2C_Stop(){
              WRITE_SDA_0;
              delay_us(3);
              WRITE_SCL_1;
              delay_us(3);
              WRITE_SDA_1;
              delay_us(3);
        }
        void clock(){
          WRITE_SCL_1;
          delay_us(5);
          WRITE_SCL_0;
          delay_us(2);
        }
        status I2C_Write(uint8_t u8Data){
              uint8_t i;
              status stRet;
              for(int i=0; i<8;i++){  //write byte data
                 if(u8Data & 0x80){
                   WRITE_SDA_1;
                 }else{
                   WRITE_SDA_0;
                 }
               delay_us(3);
               clock();
               u8Data <<= 1;
              }
              //sau khi truyen 8 bit thi SDA keo len 1
              WRITE_SDA_1;
              delay_us(3);
              
              //xung thu 9 de gui tin hieu ACK
              //trong luc gui xung thi doc trang thai cua ACK
              WRITE_SCL_1;
              delay_us(3);
              
              if(READ_SDA_VAL){ //dieu kien dung cua ìf la 1
                stRet = NOT_OK;
              }else{
                stRet = OK;
              }
              delay_us(2);
              WRITE_SCL_0;
              delay_us(3);
              
              return stRet;
        }
        uint8_t I2C_Read(ACK_Bit _ACK){
          uint8_t i;
          uint8_t u8Ret = 0x00;
          WRITE_SDA_1;
          delay_us(3);
          //doc tin hieu 8bit dau
          for(i=0;i<8;++i){
            u8Ret <<= 1;
            WRITE_SCL_1;
            delay_us(3);
                if(READ_SDA_VAL){//neu gia tri SDA la 1 thi them 1 vao thanh ghi
                  u8Ret |= 0x01;
                }
                delay_us(2);
                
            WRITE_SCL_0;
            delay_us(5);
          }
          if(_ACK){
             WRITE_SDA_0;
          }else{
             WRITE_SDA_1;		 
          }
          delay_us(3);
          
          WRITE_SCL_1;
          delay_us(5);
          WRITE_SCL_0;
          delay_us(5);
          
          return u8Ret;
        }

## b. I2C HARDWARE - TÍCH HỢP SẴN TRONG VDK
Các bước thực hiện là: Xác định các chân I2C -> Cấu hình GPIO -> Cấu hình I2C

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/3587dbc2-8f9d-44c2-b0c0-2e17dce2609d)

 - Xác định các chân GPIO

 ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/848a3a4f-1d9f-46ee-84d8-7d46636e7693)
  
 - Cấu hình GPIO

 ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/8b621881-23ed-43aa-8226-d01a33b02ba4)

 - Cấu hình I2C

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/ca7e26ce-7e88-48b9-afff-3d6ed957c639)

- Các hàm truyền nhận:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/d28ce59f-5be4-4992-ae69-bd5247062c95)

- Hàm kiểm tra cờ, sự kiện:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/97748f79-9365-4795-9331-8e54e54670ba)

## c. UART SOFTWARE (GPIO)

Các bước: ***Xác định các chân UART** -> ***Cấu hình GPIO*** -> ***Khởi tạo chân cho UART***

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/58c2a805-2e5e-4c9d-bc28-5f3a7ccafe22)

Hàm truyền nhận:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/ce41848c-c42c-4400-b1e3-91be121c5ec2)

Hàm nhận:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/c05eb440-2095-4466-92a9-ee82a56468f0)

Parity bit:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/a8229798-081d-493c-bec8-fcc8f35a523a)

Code:

            #define TX_Pin     GPIO_Pin_9
            #define RX_Pin     GPIO_Pin_10
            #define UART_GPIO  GPIOA
            //9600bits/1s <=> 9600bits/1000ms <=> 1 bit = 0,10467 = 1 period tie
            #define BRateTime  105
            typedef enum{
            	Parity_Mode_NONE,
            	Parity_Mode_ODD,
            	Parity_Mode_EVENT
            }Parity_Mode;
            void RCC_Config(){
            	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            }
            void delay_us(uint32_t delay){
            		TIM_SetCounter(TIM2,0);
            		while(TIM_GetCounter(TIM2)<delay){}
            }
            void GPIO_Config(){
            	GPIO_InitTypeDef GPIO_Config;
            	GPIO_Config.GPIO_Pin = RX_Pin;
            	GPIO_Config.GPIO_Mode = GPIO_Mode_IN_FLOATING; //cho cho toi khi kia keo xuong moi bat dau nhan nen trang thai ban dau khong xac dinh
            	GPIO_Init(GPIOA, &GPIO_Config);
            	
            	GPIO_Config.GPIO_Pin = TX_Pin;
            	GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
            	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
            	GPIO_Init(GPIOA, &GPIO_Config);
            }
            //default status of pin TX is 1
            void UART_Config(){
            	GPIO_SetBits(UART_GPIO, TX_Pin);
            	delay_us(1);
            }
            void UART_Transmit(const char DataValue){
            	GPIO_WriteBit(UART_GPIO,TX_Pin, Bit_RESET);//start bit
            	delay_us(BRateTime);
            	//dich tung bit
            	for(unsigned char i = 0; i<8;i++){
            		if(((DataValue>>i)&0x1)==0x1){
            			GPIO_WriteBit(UART_GPIO, RX_Pin, Bit_SET);
            		}else{
            			GPIO_WriteBit(UART_GPIO, RX_Pin, Bit_RESET);
            		}
            		delay_us(BRateTime);
            	}
            	//stop bit
            	GPIO_WriteBit(UART_GPIO,TX_Pin, Bit_SET);
            	delay_us(BRateTime);
            }
            unsigned char UART_Receive(void){
            	unsigned char DataValue = 0;
            	while(GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) == 1);
            	delay_us(BRateTime);
            	delay_us(BRateTime/2);
            	for ( unsigned char i = 0; i < 8; i++ ){
            		if ( GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) == 1 ){
            			DataValue += (1<<i);}
            		delay_us(BRateTime);
            		}
            		if ( GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) == 1 ){
            			delay_us(BRateTime/2);
            			return DataValue;
            		} 
            }
            uint8_t Parity_Generate(uint8_t data, Parity_Mode Mode){
            	uint8_t count =0;
            	for(int i=0; i< 8; i++){
            		if(data & 0x01){
            			count++;
            		}
            		data>>=1;
            	}
            	switch(Mode){
            		case Parity_Mode_NONE:
            			return data; 
            			break;
            		case Parity_Mode_ODD:
            			if(count%2){
            				return ((data<<1)|1);
            			} else {
            				return (data<<1);
            			}
            			break;
            		case Parity_Mode_EVENT:
            			if(!(count%2)){
            				return ((data<<1)|1);
            			} else {
            				return (data<<1);
            			}
            			break;
            		default:
            			return data;
            			break;
            	}
            }
            uint8_t Parity_Check(uint8_t data, Parity_Mode Mode){
            	uint8_t count =0;
            	for(int i=0; i< 8; i++){
            		if(data & 0x01){
            			count++;
            		}
            		data>>=1;
            	}
            	switch(Mode){
            		case Parity_Mode_NONE:
            			return 1; 
            			break;
            		case Parity_Mode_ODD:
            			return (count%2); 
            			break;
            		case Parity_Mode_EVENT:
            			return (!(count%2)); 
            			break;
            		default:
            			return 0;
            			break;
            	}
            }

## D. UART HARDWARE (GPIO)
</p>

</details>

<details><summary>LESSON 4: INTERRUPT</summary>
</p>

Interrupt - hay còn gọi là ngắt ngoài, là một tín hiệu khẩn cấp gửi đến bộ xử lí yêu cầu bộ xử lí phải tạm ngừng các hoạt động hiện tại để "nhảy" đến một nơi khác trong chương trình để thực hiện nhiệm vụ khẩn cấp nào đó - nhiệm vụ này gọi là chương trình phục vụ ngắt, ISR (Interrupt Service Routine). Sau khi xử lí xong nhiệm vụ này thì bộ đếm chương trình sẽ trả về giá trị trước đó để bộ xử lí thực hiện công việc còn đang dang dở. Như vậy, ngắt có mức độ ưu tiên cao nhất, thường xử lí các sự kiện bất ngờ nhưng không tốn thời gian. Ngắt có 2 loại: ngắt có thể xuất phát từ tín hiệu bên trong con chip(ngắt báo bộ đếm timer/counter tràn, ngắt báo quá trình gửi dữ liệu bằng RS232 kết) hoặc ngắt do các tác nhân bên ngoài(nhấn button, ngắt báo có 1 gói dữ liệu nhận được).

Ví dụ: cách tổ chức một trường trình ngắt trong chip AVR

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/1051744c-af23-4acb-92ac-0cfc61444d76)

### A. Ngắt ngoài
Ngắt ngoài là loại ngắt duy nhất độc lập với các thiết bị của chip 

+ Để sử dụng ngắt ngoài ta thực hiện các bước sau:*Xác định các chân ngắt ngoài* -> *Cấu hình các chân GPIO*

Trước tiên muốn sử dung bất cứ ngoại vi nào thì mình phải bật Clock của bus gắn với ngoại vi đó, ngoài ra phải bật thêm AFIO. AFTO là những cái funtion thay thế.

 *Bật clock:*
  
   void RCC_Config(){
  
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
    }

  *Config GPIO là input(đọc tín hiệu ngắt):*

  ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/113fe10d-ead5-4894-905a-990a154dd077)

+ Sau khi Config GPIO ở chế độ ngắt ngoài thì mình sẽ cấu hình cho EXTI
  Những tham số của ngắt ngoài bao gồm: **Line ngắt, Mode, Cạnh ngắt**
   *EXTI_Line:* Chọn line ngắt.
  
   *EXTI_Mode:* Chọn Mode cho ngắt là Ngắt(thực thi hàm ngắt) hay Sự kiện(Không thực thi)
  
   *EXTI_Trigger:* Cấu hình cạnh ngắt.
  
   *EXTI_LineCmd:* Cho phép ngắt ở Line đã cấu hình.
  
![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/069436e3-fc31-4050-83b5-84c05875d3c3)

+ Bộ NVIC cấu hình các tham số ngắt và quản lý các vector ngắt. Các tham số trong bộ này gồm:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/15d6d9c6-2431-4036-90d3-92496b687d26)
  
Ví dụ về cấu hình NVIC:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/05d21012-a698-4ec0-a871-9fd14583fc82)


Hàm phụ vụ ngắt ngoài: ***EXTIx_IRQHandler()*** (x là line ngắt tương ứng), ***Hàm EXTI_GetITStatus(EXTI_Linex)*** :Kiểm tra cờ ngắt của line x tương ứng. 
***Hàm EXTI_ClearITPendingBit(EXTI_Linex)***: Xóa cờ ngắt ở line x.

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/5e8e2976-0f6d-4574-80cf-52f29ee3a0bd)

Tóm lại: Cấu hình GPIO ở chế độ ngắt ngoài -> Cấu hình EXTI(line, Ngắt(thực thi) or Sự kiện(không thực thi), cạnh ngắt, cho phép ngắt ở cạnh đó) -> Bộ quản lí các thamm số và quản lí vector ngắt NVIC (Vector Line, độ ưu tiên chính, độ ưu tiên phụ, cho phép ngắt).

</p>

</details>

<details><summary>LESSON 4: ADC</summary>
</p>
Vi điều khiển hay các thiết bị ngày nay đều sử dụng tín hiệu số dựa trên các bit nhị phân để hoạt động. Còn thực tế thì không chỉ mãi là tín hiệu số mà là tín hiệu tương tự và liên tục vì vậy cần phải có thiết bị chuyển đổi từ tín hiệu tương tự sang tín hiệu số. 
*ADC - Analog to Digital Convert*: bộ chuyển đổi từ tín hiệu tương tự sang tín hiệu số 

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/6489b83b-dd9c-4a2a-9ed0-e8298c28b88d)

Khả năng chuyển đổi của ADC phụ thuộc vào 2 yếu tố
+ Độ phân giải: Số bit mà ADC sử dụng để mã hóa tín hiệu. Hay còn gọi là số mức tín hiệu được biểu diễn(có độ phân giải càng cao thì độ chính xác càng lớn.)
+ Tần số/Chu kì lấy mẫu: tốc độ/khoảng thời gian giữa 2 lần mã hóa(tần số lấy mẫu càng cao thì chuyển đổi sẽ có độ chính xác càng lớn). Tần số lấy mẫu = 1/(thời gian lấy mẫu + thời gian chuyển đổi).
### ADC trong stm32
Trong stm32 có 2 kênh ADC đó là ADC1 và ADC2, mỗi bộ có tối đa 9 channel với nhiều mode hoạt động, kết quả chuyển đổi được lưu trong thanh ghi 16 bit.
- Độ phân giải: 12 bit
- Có các ngắt hổ trợ, có thể điều khiển hoạt động ADC bằng xung Trigger.
- Thời gian chuyển đổi nhanh: 1us tại tần số 65Mhz.
- Có bộ DMA giúp tăng tốc độ xử lí
  Sơ đồ khối bộ ADC:
  
  ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/7a8be06e-9ff2-4540-af12-c489ac25415e)

***Cấu hình GPIO -> Cấu hình ADC*** 
+ Các chế độ của ADC:
  >- Single: ADC chỉ đọc 1 kênh duy nhất, và chỉ đọc khi kênh nào được yêu câu.
  >- Single Continous: sẽ đọc 1 kênh duy nhất, nhưng đọc dữ liệu nhiều lần
  >- Scan: Multi-Channels: Quét qua và đọc dữ liệu nhiều kênh, nhưng chỉ đọc khi nào được yêu cầu.
  >- Scan: Continous Multi-Channels Repeat: Quét qua và đọc dữ liệu nhiều kênh, nhưng đọc liên tiếp nhiều lần giống như Single Continous.

+ Các tham số:

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/4b56e05e-1ffe-4c63-ace0-3a56b127321f)



</p>

</details>


<details><summary>LESSON 5: DMA</summary>
</p>

</p>

</details>

<details><summary>LESSON 6: Flash & BootLoader</summary>
</p>
  
*Nguồn tham khảo: Lập trình điện tử*
Chắc hẳn các bạn đã từng nghe quá trình ***boot*** trong máy tính, nó là một phần quan trọng giúp cho việc khởi động hệ điều hành máy tính. Bootloader là phần mềm quan trọng nạp vào máy tính trước khi khởi động.
Ta học vi điều khiển phải cần biến đến Bootloader 👉Rỏ ràng máy tính cũng xử dụng nhiều vi xử lí, Bootloader như một firmware nạp sẵn, trước khi máy tính khởi động thì cái firmware này sẽ được gọi, Vi điểu khiển của chúng ta cũng sử dụng lõi vi xử và khi khởi động thì nó sẽ bắt đầu từ đâu?

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/0ea09278-006d-4c9b-8529-67bbcf23f45c)

Nếu có một chương trình bootloader nạp vào trong vi điều khiển, thì trước tiên nó nhảy vào chương trình bootloader thực hiện một số công việc, sau đó mới thực hiện Application.
 #### Tổ chức bộ nhớ STM32
- Hiểu được tổ chức bộ nhớ STM32 rất quan trọng trong bootloader. Chúng ta cần nắm chương trình **boot** nằm ở địa chỉ nào, chương trình nằm ở địa chỉ nào và cách phân chia Page của bộ nhớ FLash(stm32f1). Trong vi điều khiển STM có 2 bộ nhớ cần được quan tâm là: bộ nhớ chương trình và bộ nhớ dữ liệu:
     >- Vùng nhớ code: có thể là FLASH/EEPROM/ROM/OTP,... dùng để lưu code và các lệnh của chương trình
     >- Vùng nhớ SRAM: sử dụng để kế nối Sram trên chip, dùng để lưu dữ liệu tạm thời khi run-time.
  
![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/8f71965a-ac75-4e2c-b051-857527b898be)

 👉 Địa chỉ bộ nhớ Flash bắt đầu 0x00000000 nhưng trong vi điều khiển STM32, vùng nhớ code bắt đầu từ địa chỉ 0x0800 0000 khi mình nạp xuống thì nó sẽ mặc định nạp chương trình từ địa chỉ này, với MSP ở 0x0800 0000 và Vector Table bắt đầu từ địa chỉ 0x0800 0004 (Reset_Handler).
 
 ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/1d57354c-7b16-46b3-b4a6-f71e67cd7e63)

Vi điều khiển STM32F1 cung cấp 128/64Kb, ngoài lưu trữ MSP, Vector Table, bộ nhớ Flash sẽ lưu trữ vùng nhớ chương trình ứng dụng của chúng ta, cùng với đó là vùng data.

👉Để thao tác với bộ nhớ hiệu quả thì bộ nhớ Flash trong STM32 chia thành các Page, mỗi Page có kích thước 1Kb.

👉Bộ nhớ Flash có thể được thao tác ghi **trên từng word**(2bytes/4bytes) nhưng lại chỉ có thể xóa theo từng Page
=>Vì vậy , chúng ta có thể thực hiện Bootloader bằng cách cài đặt chương trình Bootloader ở một Page nào đó, chẳng hạn như Page0, Và cùng lúc đó đặt Firmware application 1 vào Page1, Firmware application 2 vào Page2, Firmware application 3 vào Page3.

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/200f08ce-a419-472b-989d-d4387b2511de)

 👉Chúng ta sẽ bắt đầu với chương trình Bootloader, được đặt tại địa chỉ 0x0800 0000. Ngoài ra, chúng ta thấy sự xuất hiện của 3 Firmware khác:
 
 + *Factory Firmare:* là phiên bản đầu tiên của Firmware mà nhà sản xuất cung cấp cho người dùng.
 + *Current Firmware:* là phiên bản hiện tại của Firmware đang chạy trên vi điều khiển, được chúng ta lưu trên 1 Page nào đó.
 + *FOTA Firmware:* là phiên bản cập nhật mới của firmware.

### Bootloader chủ yếu quan tâm đến nạp file nhị phân xuống bộ nhớ của Vi điều khiển, có 2 vấn đề cần được chú ý đó là: Sắp xếp chúng trong bộ nhớ như nào và File nhị phân (cấu trúc, nội dung).

 #### Little Endian and Big Endian
 Most modern computer memory are byte addressible. Each byte in memory has its own unique address. 
 If a data object take multiple contiguous bytes in memory, the memory address of this object is defined as the lowest address of all bytes this object contains. 
 
 ![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/0b203571-5e92-4934-9c18-c748b70c6e58)

Ví dụ: một word gồm 4 bytes được khoanh như hình, mỗi bytes đều có một địa chỉ của chính nó, nhưng địa chỉ của cái word này là địa chỉ của byte thấp nhất trong 4 byte này là 0x20000000.
*Vậy word được lưu trữ tại 0x20000000 là bao nhiêu?* 

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/bf1a5dd8-8208-421d-bb1a-74e223ebab53)

The anwser is that it depends on endian.
- Big endian:(big - end: lớn cuối cùng - địa chỉ lớn cuối cùng =>giá trị lớn cuối cùng) thứ tự sắp xếp theo chiều từ nơi địa chỉ thấp nhất đến cao nhất =>if big endian thì value = 01020304
- Little endian: ngược lại =>value = 04030201
 #### File nhị phân
 Có một số định dạng file nhị phân thường gặp là: .BIN, .ELF, .HEX
 + .BIN: định dạng cơ bản nhất của file nhị phân, không cần chính sửa hay trải qua quá trình relocation, câu lệnh sẽ được tải trực tiếp xuống địa chỉ. 
 + .ELF: định dạng này cần thêm quá trình relocation. Chúng ta có thể nạp vào địa chỉ mà ta mong muốn.
 + .HEX: định dạng này khá quen thuộc với chúng ta, thường được gen ra từ phần mềm
👉 Cần nắm được cấu trúc của .HEX
Chúng ta làm việc với Bootloader nên việc nạp code không thông qua phần mềm hay IDE nữa, chúng ta nghĩ đến việc nạp code thông qua mạng không dây hoặc qua các giao thức SPI,I2C,UART.
Khi nạp code thông qua phần mềm, thì phần mềm phải phân tích file Hex rồi tải data xuống Flash thông qua ST-Link. Còn ta tự làm thì phải **phân tích file hex**->**tải data xuống Flash** thông qua mạng không giây hoặc các Protocol khác =>***Việc hiểu file .HEX rất quan trọng***

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/778e6bae-3aa8-48db-a18b-259337697e12)

- Start Code: Ở mỗi dòng sẽ luôn bắt đầu bằng dấu hai chấm ':'
- Byte Count: Ở đây bao gồm 2 chữ số (1 byte) để chỉ thị số byte data có trên dòng tương ứng. 
- Address: 4 chữ số, đây là địa chỉ offset, địa chỉ này cộng thêm địa chỉ base (chúng ta có thể chọn - ví dụ 0x0800.0000 chẳng hạn) sẽ ra địa chỉ mà data được nạp xuống bộ nhớ FLASH. 
- Record Type: 2 chữ số, từ 00 đến 05, nói lên ý nghĩa của data. 

![image](https://github.com/NguyenNgocQuyen29/Embedded-System/assets/124705679/ce837fe8-67de-42ef-a245-3f5558d5ce9b)


- Data: Phần này là dữ liệu sẽ lưu lên FLASH, số byte sẽ được quy định ở trường Byte Count.
 Checksum: gồm 2 chữ số, dùng để kiểm tra lỗi. Theo quy định thì một line sẽ đúng khi mà byte checksum sẽ bằng ***đảo*** của tổng tất cả các byte còn lại cùng dòng, cộng thêm 1. 
#### Thao tác trên bộ nhớ Flash
Khi làm việt với bootloader chúng ta cần quan tâm xem mình cần làm gì (Data từ file Hex) -> ghi vào đâu? (Address của file Hex).
- Có 3 loại thao tác với bộ nhớ Flash: đọc - Read, ghi - Write, xóa - Delete.

Bài tập: flash - bootloader

</p>

</details>

<details><summary>LESSON 7: CAN</summary>
</p>

</p>

</details>

<details><summary>Một vài câu hỏi ôn tập</summary>
</p>
  
## C/C++

### 1. Destructor trong Class có thể bị overloaded không?
- Không thể bị overload(nạp chồng).
- Nguyên nhân: trong một class chỉ có 1 destructor.

### 2. Function overloading và operator overloading?
- Function loading là một tính năng của ngôn ngữ C(không có dùng trong C). Kỹ thuật này cho phép sử dụng cùng một tên gọi cho nhiều hàm(có cùng mục đích) nhưng khác kiểu dữ liệu tham số hoặc số lượng tham số truyền vào. Cùng kiểu dữ liệu trả về và tham số truyền vào là tham trị không phải là tham chiếu.
- operator overloading: là 1 tính năng trong c++, cho phép thực hiện các toán tử tạo ra từ class.

### 3. So sánh struct với class
- Mặc định: trong struct mặc định là public, một class mặc định truy cập là private.
- Mục đích: struct đại diện cho một cấu trúc dữ liệu đơn giản, trong khi class được sử dụng để tạo  một đối tương. 
### 4. Từ khóa typedef và define
- Typedef(type defination)- định nghĩa kiểu:kết thúc bằng dấu *;* , nó đơn giản chỉ là, sử dụng một cái tên mới để thay thế cho những kiểu dữ liệu đã cs=ó trong ngôn ngữ lập trình C/C++, được sử lí bởi *compiler*.
- Define: không có dấu ; cuối cùng, define có thể sử dụng định để định nghĩa kiểu dl giống typedef,  ngoài ra nó cũng có thể định nghĩa một giá trị, hoặc cũng có thể định nghĩa cấu trúc như for or những câu lệnh, được sử lí bởi quá trình *preprocessor.*
### 5. Mặc định
- Nếu chúng ta chưa khai báo giá trị thì đối với giá trị số nó sẽ tự khai báo với giá trị là 0, còn với kí tự thì nó khai báo với giá trị là NULL.
### 6. Inline Function
- key: inline
- Khi dùng hàm này thì tác dụng của nó sẽ bỏ qua quá trình gọi hàm thông thường , thay vì gọi hàm thì nó sẽ thực thi luôn
  => tốc độ nhanh hơn, nhưng nếu dùng nhiều nó làm cho cái kích thước của bộ nhớ lớn.
Lưu ý: Khi dùng inline , hàm của nó chỉ 1 dòng ,1 phép toán đơn giản chứ nếu phức tạp quá thì không nên sử dụng hàm inline.
### 6. Kích thước struct, union, enum?
- Struct: bằng tổng khối lượng của các phần tử cộng với padding nếu có.
- Union: bằng kích thước của phần tử lớn nhất cộng với padding nếu
- Enum: bằng với kích thước kiểu int (tùy thuộc vào cấu trúc của vi xử lí máy tính là 32bit hay 64bit, 32bit:4byte, 64bit:8 byte)
## Embedded 

### 1. Làm sao để biết data gửi đi trong SPI,I2C là đúng?
- Thêm 1 bit check sum data.
- Thuật toán như CRC.
### 2. DMA khác gì với ngắt truyền thông?
- Khi mình sử dụng DMA thì dữ liệu được gửi đến hoặc truyền đi được xử không có sự can thiệp của CPU, tức là nó hoạt động gần như là song song với CPU.
- Bản chất của ngắt là đợi một tín hiệu ngắt truyền đến hoặc gửi đi và nó sử lí những tác vụ ngắt tưng ứng với tín hiệu ngắt, tức là CPU làm việc
  => Tốc độc của DMA nhanh hơn vì không có sự can thiệp của CPU, còn ngắt thì đòi hỏi CPU phải xử lí dữ liệu mỗi khi có ngắt xảy ra.

</p>

</details>
   


    
  


