# Project 3: Layered Network Protocols
## Group members: Brian Nguyen and XD


## Overall Design
### Layer 2: Messages
#### The sender:
In this layer, the sender sends the message. The message needs to have a header. We decided to put the length of the message inside the header. Since the maximum length of the message can be 1024, it needs 2 bytes to represent the length of the messages. Hence, we made char header[2]. We did the necessary calculations to convert the length from integer to char. The sender then sends the header to the receiver using l1_write. After that, the sender sends the message. Since l1_write can only write 1 byte at a time, we used a for loop to send the entire message. 

#### The receiver: 
In this layer, the receiver receives the buffer that has the message and the header, which contains the length of the message. Since we know that the header takes 2 bytes, we read the first 2 bytes in a char header[2]. Since the header contains the length of the message, we did the necessary calculations the convert the length of the message from 2-byte header to an integer value. We compare the message length obtained from the header with maxlength. If the length of the message exceeds maxlength, the function returns -1. Otherwise, the function reads the message with the help of a for loop then return the length of the message. 

### Layer 3: Messages with Error Detection
#### The sender:
In this layer, the sender sends the message along with an error detection mechanism. The error detection mechanism we decided to use is checksum. However, our implementation is a bit different from the book. We don't convert the message into 2-byte chunks. We cast the output of checksum to char data type because we wanted to store the output of checksum as the header of the message. We concatenate the header with the message, since l2_write does not send byte by byte, then use l2_write to send the whote thing.

#### The receiver:
In this layer, the receiver receives the buffer that has both the message and the header, which contains the output of checksum. Firstly, we read the entire buffer using l2_read. If l2_read return -1, l3_read also return -1. Otherwise, the return of l2_read is also the length of the message. We compare the length of the message with maxlength, if the length of the message exceeds maxlength, the function returns -1. We then performs checksum on the buffer. If the output of checksum equals 0, meaning there is no error, the function returns the size of the message. 

### Layer 4: Name/Value Pairs
