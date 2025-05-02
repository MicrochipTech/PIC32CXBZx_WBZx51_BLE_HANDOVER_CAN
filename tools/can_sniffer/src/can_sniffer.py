#!/usr/bin/python
import colorama, logging, msvcrt, queue, re, serial, threading, time

COM_PORT = 'COM61'

class SamE54XproTest(object):
    node_id_dict = {
        '[045A]':'Broadcast',
        '[045B]':'Node #1',
        '[045C]':'Node #2',
        '[045D]':'Node #3',
        '[045E]':'Node #4'
    }
    rssi = [-127, -127, -127, -127]

    def __init__(self):
        '''
        Constructor
        '''
        format = "%(asctime)s: %(message)s"
        logging.basicConfig(format=format, level=logging.INFO, datefmt="%H:%M:%S")        
        logging.info('__init__()')

        self.node = ''

        self.ser = serial.Serial(port=COM_PORT, baudrate=921600, timeout=0.1)

        colorama.init(autoreset=True)

        # --M43814-- timestr = time.strftime("%Y%m%d_%H%M%S")
        # --M43814-- self.filename = 'log_' + timestr + '.csv'

    def __del__(self):
        '''
        Destructor
        '''
        pass

    def run(self):
        '''
        Main Routine used for Command/Event decoding
        '''
        while True:
            msg = self.ser.readline()
            msg = msg.decode().strip()
            # logging.info(msg)
            if len(msg) > 0:
                parameters =  msg.split(' ')
                # logging.info(parameters)
                id = parameters[0]
                data = []
                for p in parameters[1:]:
                    data.append( int(p,16) )
                self.node = self.node_id_dict[id] 
                self.decode_packet(data)            
                    
            if msvcrt.kbhit():
                break
    # -------------------------------------------------------------------------
    # -------------------------------------------------------------------------
    # HCI packets
    # -------------------------------------------------------------------------
    # -------------------------------------------------------------------------
    def decode_packet(self,data):
        '''
        HCI Packet decoding
        '''
        packet_type = data[0]
        if packet_type == 0x01 :
            # 0x01 - HCI Command Packet
            self.decode_command(data)
        elif packet_type == 0x04:
            # 0x04 - HCI Event Packet
            self.decode_event(data)
        else:
            # 0x02 - HCI ACL Data Packet
            # 0x03 - HCI Synchronous Data Packet
            # 0x05 - HCI ISO Data Packet
            print ( data )

    # -------------------------------------------------------------------------
    # -------------------------------------------------------------------------
    #   HCI Command Packets
    # -------------------------------------------------------------------------
    # -------------------------------------------------------------------------
    def decode_command(self, data):
        '''
        HCI Command Packet decoding
        '''
        # print ('Command')
        op_code = data[1] + (data[2] << 8) 
        ogf = op_code >> 10
        ocf = op_code & 0x03FF
        parameter_total_length = data[3]
        if ogf == 0x03:
            if ocf == 0x03:
                print('\n{0}:HCI_Reset Command'.format(self.node))
        elif ogf == 0x3F:
            if ocf == 0xE2:
                print('\n{0}:HCI_VENDOR_SPECIFIC_HANDOVER_1'.format(self.node))
            if ocf == 0xE3:
                print('\n{0}:HCI_VENDOR_SPECIFIC_HANDOVER_2'.format(self.node))
            if ocf == 0xE4:
                id = data[4] + (data[5] << 8)
                print(colorama.Fore.RED + '\n\n{0}:HCI_VENDOR_SPECIFIC_HANDOVER_3:{1:04X}'.format(self.node,id),end='\n\n')
        else:
            logging.error('Decoding of Command Complete Event OGF={0:02X} Not implemented'.format(ogf))
            logging.error('Command op_code:{0:04X} ogf:{1:X} ocf:{2:X}'.format(op_code,ogf,ocf))
            logging.error( data )

    # -------------------------------------------------------------------------
    # -------------------------------------------------------------------------
    #   HCI Event Packets
    # -------------------------------------------------------------------------
    # -------------------------------------------------------------------------
    def decode_hci_connection_complete_event(self,data):
        '''
        HCI_Connection_Complete event decoding
        '''
        status = data[0]
        handle = data[1]
        bd_addr = data[2:7]
        link_type = data[8]
        encryption_enabled = data[9]
        print('\n{0}:HCI_Connection_Complete:'.format(self.node),end='')
        print('{0:02X} '.format(status),end='')
        print('{0:04X} '.format(handle),end='')
        print(''.join('{:02X} '.format(x) for x in bd_addr),end='')
        print('{0:02X} '.format(link_type),end='')
        print('{0:02X} '.format(encryption_enabled))
        

    def decode_hci_disconnection_complete_event(self,data):
        '''
        HCI_Disconnection_Complete event decoding
        '''
        status = data[0]
        handle = data[1] + (data[2] << 8)
        reason = data[3]
        print('\n{0}:HCI_Disconnection_Complete:{1:02X} {2:04X} {3:02X}'.format(self.node, status, handle, reason))
        
    def decode_hci_command_complete_event(self,data):
        '''
        HCI_Command_Complete event decoding
        '''
        op_code = data[1] + (data[2] << 8) 
        ogf = op_code >> 10
        ocf = op_code & 0x03FF
        if ogf == 0x03:
            if ocf == 0x03:
                status = data[3]
                print('\n{0}:HCI_Reset Command Complete:{1:02X}'.format(self.node,status))
        elif ogf == 0x05:
            if ocf == 0x05:
                status = data[3]
                handle = data[4] + (data[5] << 8)
                rssi = data[6]
                if rssi > 128:
                    rssi = rssi-256
                
                # update rssi 
                if 'Node #1' in self.node:
                    update_index = 0
                    self.rssi[0] = rssi
                elif 'Node #2' in self.node:
                    update_index = 1
                    self.rssi[1] = rssi
                elif 'Node #3' in self.node:
                    update_index = 2
                    self.rssi[2] = rssi
                elif 'Node #4' in self.node:
                    update_index = 3
                    self.rssi[3] = rssi
                else:
                    pass

                max_index = self.rssi.index(max(self.rssi))                
                
                i = 0
                print ( '{0}:HCI_Read_RSSI Command Complete:'.format(self.node), end=' ' )
                for r in self.rssi:
                    if i == max_index:
                        print (colorama.Fore.GREEN + '{0}'.format(r), end=' ')
                    elif i == update_index:
                        print (colorama.Fore.YELLOW + '{0}'.format(r), end=' ')
                    else:
                        print (colorama.Fore.WHITE + '{0}'.format(r), end=' ')
                    i = i+1
                print ( '',end='\r' )
                
                # with open(self.filename, 'a') as f:
                #     f.write('{0};{1};{2};{3}\n'.format(self.rssi[0],self.rssi[1],self.rssi[2],self.rssi[3]))

        else:
            logging.error('Decoding of Command Complete Event OGF={0:02X} Not implemented'.format(ogf))
            logging.error('op_code:{0:04X} ogf:{1:X} ocf:{2:X}'.format(op_code,ogf,ocf))
            logging.error(data)

    '''
    Event Code dictionary
    '''
    event_code_dict = {
        0x03 : ['HCI_Connection_Complete', decode_hci_connection_complete_event],
        0x05 : ['HCI_Disconnection_Complete', decode_hci_disconnection_complete_event],
        0x0E : ['HCI_Command_Complete', decode_hci_command_complete_event],
    }

    def decode_event(self, data):
        '''
        HCI Event Packet decoding
        '''
        # print ('Event')
        event_code = data[1]
        parameter_total_length = data[2]

        if event_code in self.event_code_dict.keys():
            event = self.event_code_dict[event_code][0]
            if 'HCI_Connection_Complete' in event:
                self.decode_hci_connection_complete_event(data[3:3+parameter_total_length])
            elif 'HCI_Command_Complete' in event:
                self.decode_hci_command_complete_event(data[3:3+parameter_total_length])
            elif 'HCI_Disconnection_Complete':
                self.decode_hci_disconnection_complete_event(data[3:3+parameter_total_length])
            else:
                logging.error('Decoding of Event {0} Not implemented'.format(event))
        else:
            logging.error('Unknown Event Code: {0:02X}'.format(event_code))
            logging.error( data )
        
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# Main
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
if __name__ == '__main__':
    '''
    Main
    '''
    sam = SamE54XproTest()
    sam.run()
    print('\r\nEnd of the game\r\n')