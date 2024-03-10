#  **I. STM32**
## 1. GPIO ##
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
    
   


    
  


