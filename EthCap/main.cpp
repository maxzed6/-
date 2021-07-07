#include <iostream>
#include <pcap.h>
#include <cstring>

using namespace std;

struct ether_header{
    u_int8_t ether_dhost[6];
    u_int8_t ether_shost[6];
    u_int16_t ether_type;
};
struct arp_header{
    u_int16_t arp_hardware_type;
    u_int16_t arp_protocol_type;
    u_int8_t arp_hardware_length;
    u_int8_t arp_protocol_length;
    u_int16_t arp_operation_code;
    u_int8_t arp_source_ethernet_address[6];
    u_int8_t arp_source_ip_address[4];
    u_int8_t arp_destination_ethernet_address[6];
    u_int8_t arp_destination_ip_address[4];
};
struct ip_header{
#ifdef WORDS_BIGENDIAN
    u_int8_t ipversion:4,
            ip_header_length:4;
#else
    u_int8_t ip_header_length:4,
            ip_version:4;
#endif
    u_int8_t ip_tos;
    u_int16_t ip_length;
    u_int16_t ip_id;
    u_int16_t ip_off;
    u_int8_t ip_ttl;
    u_int8_t ip_protocol;
    u_int16_t ip_checksum;
    struct in_addr ip_source_address;
    struct in_addr ip_destination_address;
};
//struct ipv6_header{
//#ifdef WORDS_BIGENDIAN
//    u_int32_t ipversion:4,
//            ipv6_traffic_class:8,
//            ipv6_flow_label:20;
//#else
//    u_int32_t
//    ipv6_flow_label:20,
//            ipv6_traffic_class:8,
//            ipversion:4;
//
//
//#endif
//    u_int16_t ipv6_payload_length;
//    u_int8_t ipv6_next_header;
//    u_int8_t ipv6_hop_limit;
//
//    struct in_addr ipv6_source_address;
//    struct in_addr ipv6_destination_address;
//};
struct tcp_header{
    u_int16_t tcp_source_port;
    u_int16_t tcp_destination_port;
    u_int32_t tcp_acknowledgement;
    u_int32_t tcp_ack;
#ifdef WORDS_BIGENDIAN
    u_int8_t tcp_offset:4,
    tcp_reserved:4;
#else
    u_int8_t tcp_reserved:4,
    tcp_offset:4;
#endif
    u_int8_t tcp_flags;
    u_int16_t tcp_windows;
    u_int16_t tcp_checksum;
    u_int16_t tcp_urgent_pointer;
};
struct udp_header{
    u_int16_t udp_source_port;
    u_int16_t udp_destination_port;
    u_int16_t udp_length;
    u_int16_t udp_checksum;
};
struct icmp_header{
    u_int8_t icmp_type;
    u_int8_t icmp_code;
    u_int16_t icmp_checksum;
    u_int16_t icmp_id;
    u_int16_t icmp_sequence;
};

char error_content[PCAP_ERRBUF_SIZE];
char *net_interface = pcap_lookupdev(error_content);

void packet_callback(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content){
    static int packet_number = 1;
    printf("The %d packet is captured.\n", packet_number);
    packet_number++;
}

//void ipv6_protocol_packet_callback(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content){
//    struct ipv6_header *ipv6_protocol;
//    ipv6_protocol = (struct ipv6_header*)(packet_content+14);
//    printf("******************IPV6 PROTOCOL***********************\n");
//    printf("IP version:%d\n", ipv6_protocol->ipversion);
//    printf("IPv6 Traffic Class:%d\n", ipv6_protocol->ipv6_traffic_class);
//    printf("IPv6 Flow Label:%d\n", ipv6_protocol->ipv6_flow_label);
//    printf("IPv6 Payload Length:%d\n", ipv6_protocol->ipv6_payload_length);
//    printf("IPv6 Next Header:%d\n", ipv6_protocol->ipv6_next_header);
//    printf("IPv6 Hop Limit:%d\n", ipv6_protocol->ipv6_hop_limit);
//    printf("IPv6 Source IP Address:%s\n", inet_ntoa(ipv6_protocol->ipv6_source_address));
//    printf("IPv6 Destination IP Address:%s\n", inet_ntoa(ipv6_protocol->ipv6_destination_address));
//}

void icmp_protocol_packet_callback(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content){
    struct icmp_header *icmp_protocol;
    icmp_protocol = (struct icmp_header*)(packet_content+14+20);
    printf("******************ICMP PROTOCOL***********************\n");
    printf("ICMP Type:%d\n", icmp_protocol->icmp_type);
    switch (icmp_protocol->icmp_type) {
        case 8:
            printf("ICMP Echo Request Protocol\n");
            break;
        case 0:
            printf("ICMP Echo Reply Protocol \n");
            break;
        default:
            break;
    }
    printf("ICMP Code:%d\n", icmp_protocol->icmp_code);
    printf("Identifier:%d\n", icmp_protocol->icmp_id);
    printf("Sequence Number:%d\n", icmp_protocol->icmp_sequence);
    printf("ICMP Checksum:%d\n", ntohs(icmp_protocol->icmp_checksum));
}

void udp_protocol_packet_callback(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content){
    struct udp_header *udp_protocol;
    u_short source_port;
    u_short destination_port;
    u_short length;
    udp_protocol = (struct udp_header*)(packet_content+14+20);
    source_port = ntohs(udp_protocol->udp_source_port);
    destination_port = ntohs(udp_protocol->udp_destination_port);
    length = ntohs(udp_protocol->udp_length);
    printf("******************UDP PROTOCOL***********************\n");
    printf("Source port:%d\n", source_port);
    printf("Destination port:%d\n", destination_port);
    switch (destination_port) {
        case 138:
            printf("NETBIOS Datagram Service\n");
            break;
        case 137:
            printf("NETBIOS Name Service\n");
            break;
        case 139:
            printf("NETBIOS session service\n");
        case 53:
            printf("name-domain server\n");
            break;
        default:
            break;
    }
    printf("Length:%d\n", length);
    printf("Checksum:%d\n", ntohs(udp_protocol->udp_checksum));
}

void tcp_protocol_packet_callback(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content){
    struct tcp_header *tcp_protocol;
    u_char flags;
    int header_length;
    u_short source_port;
    u_short destination_port;
    u_short windows;
    u_short urgent_pointer;
    u_int sequence;
    u_int acknowledgement;
    u_int16_t checksum;
    tcp_protocol = (struct tcp_header*)(packet_content+14+20);
    source_port = ntohs(tcp_protocol->tcp_source_port);
    destination_port = ntohs(tcp_protocol->tcp_destination_port);
    header_length = tcp_protocol->tcp_offset * 4;
    sequence = ntohl(tcp_protocol->tcp_acknowledgement);
    acknowledgement = ntohl(tcp_protocol->tcp_ack);
    windows = ntohs(tcp_protocol->tcp_windows);
    urgent_pointer = ntohs(tcp_protocol->tcp_urgent_pointer);
    flags = tcp_protocol->tcp_flags;
    checksum = ntohs(tcp_protocol->tcp_checksum);
    printf("******************TCP PROTOCOL***********************\n");
    printf("Source Port:%d\n", source_port);
    printf("Destination Port:\n", destination_port);
    switch (destination_port) {
        case 80:
            printf("HTTP protocol\n");
            break;
        case 21:
            printf("FTP protocol\n");
            break;
        case 23:
            printf("TELNET protocol\n");
            break;
        case 25:
            printf("SMTP protocol\n");
            break;
        case 110:
            printf("POP3 protocol\n");
            break;
        default:
            break;
    }
    printf("Sequence Number:%d\n", sequence);
    printf("Acknowledgement Number:%u\n", acknowledgement);
    printf("Header Length:%d\n", header_length);
    printf("Reserved:%d\n", tcp_protocol->tcp_reserved);
    printf("Flags:");
    if (flags & 0x08) printf("PSH ");
    if (flags & 0x10) printf("ACK ");
    if (flags & 0x02) printf("SYN ");
    if (flags & 0x20) printf("URG ");
    if (flags & 0x01) printf("FIN ");
    if (flags & 0x04) printf("RST ");
    printf("\n");
    printf("Window Size:%d\n", windows);
    printf("Checksum:%d\n", checksum);
    printf("Urgent pointer:%d\n", urgent_pointer);
}

void ip_protocol_packet_callback(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content){
    struct ip_header *ip_protocol;
    u_int header_length;
    u_int offset;
    u_int tos;
    u_int16_t checksum;
    ip_protocol = (struct ip_header*)(packet_content+14);
    checksum = ntohs(ip_protocol->ip_checksum);
    header_length = ip_protocol->ip_header_length * 4;
    tos = ip_protocol->ip_tos;
    offset = ntohs(ip_protocol->ip_off);
    printf("******************IP PROTOCOL***********************\n");
    printf("IP Version:%d\n", ip_protocol->ip_version);
    printf("Header length:%d\n", header_length);
    printf("TOS:%d\n", tos);
    printf("Total length:%d\n", ntohs(ip_protocol->ip_length));
    printf("Identification:%d\n", ntohs(ip_protocol->ip_id));
    printf("Offset:%d\n", (offset & 0x1fff) * 8);
    printf("TTL:%d\n", ip_protocol->ip_ttl);
    printf("Protocol:%d\n", ip_protocol->ip_protocol);
    switch (ip_protocol->ip_protocol) {
        case 6:
            printf("The Transport Layer Protocol is TCP\n");
            break;
        case 17:
            printf("The Transport Layer Protocol is UDP\n");
            break;
        case 1:
            printf("The Transport Layer Protocol is ICMP\n");
            break;
        default:
            break;
    }
    printf("Header checksum:%d\n", checksum);
    printf("Source address:%s\n", inet_ntoa(ip_protocol->ip_source_address));
    printf("Destination address:%s\n", inet_ntoa(ip_protocol->ip_destination_address));
    switch (ip_protocol->ip_protocol) {
        case 6:
            tcp_protocol_packet_callback(argument, packet_header, packet_content);
            break;
        case 17:
            udp_protocol_packet_callback(argument, packet_header, packet_content);
            break;
        case 1:
            icmp_protocol_packet_callback(argument, packet_header, packet_content);
            break;
    }
}

void arp_protocol_packet_callback(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content){
    struct arp_header *arp_protocol;
    u_short protocol_type;
    u_short hardware_type;
    u_short operation_code;
    u_char *mac_string;
    struct in_addr source_ip_address;
    struct in_addr destination_ip_address;
    u_char hardware_length;
    u_char protocol_length;
    printf("******************ARP PROTOCOL***********************\n");
    arp_protocol = (struct arp_header*)(packet_content+14);
    hardware_type = htons(arp_protocol->arp_hardware_type);
    protocol_type = ntohs(arp_protocol->arp_protocol_type);
    operation_code = ntohs(arp_protocol->arp_operation_code);
    hardware_length = arp_protocol->arp_hardware_length;
    protocol_length = arp_protocol->arp_protocol_length;
    printf("ARP Hardware Type:%d\n", hardware_type);
    printf("ARP Protocol Type:%d\n", protocol_type);
    printf("ARP Hardware Length:%d\n", hardware_length);
    printf("ARP Protocol Length:%d\n", protocol_length);
    printf("ARP Operation:%d\n", operation_code);
    switch (operation_code) {
        case 1:
        case 3:
            printf("ARP Request Protocol\n");
            break;
        case 2:
        case 4:
            printf("ARP Reply Protocol\n");
            break;
        default:
            break;
    }
    printf("Mac Destination Address is: \n");
    mac_string = arp_protocol->arp_source_ethernet_address;
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *mac_string, *(mac_string+1), *(mac_string+2), *(mac_string+3), *(mac_string+4), *(mac_string+5));
    memcpy((void*)&source_ip_address, (void*)&arp_protocol->arp_source_ip_address, sizeof(struct in_addr));
    printf("Source IP Address:%s\n", inet_ntoa(source_ip_address));
    printf("Ethernet Destination Address is:\n");
    mac_string = arp_protocol->arp_destination_ethernet_address;
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *mac_string, *(mac_string+1), *(mac_string+2), *(mac_string+3), *(mac_string+4), *(mac_string+5));
    memcpy((void*)&destination_ip_address, (void*)&arp_protocol->arp_destination_ip_address, sizeof(struct in_addr));
    printf("Destination IP Address:%s\n", inet_ntoa(destination_ip_address));
}


void ethernet_protocol_packet_callback(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content){
    u_short ethernet_type;
    struct ether_header *ethernet_protocol;
    u_char *mac_string;
    static int eth_packet = 1;
    printf("******************ETHERNET PROTOCOL***********************\n");
    printf("Capture %dth Packet from net_interface : \n", eth_packet++);
    printf("%s\n", net_interface);
    printf("Capture Time is:\n");
    printf("%s", ctime((const time_t*)&packet_header->ts.tv_sec));
    printf("Packet Length is:\n");
    printf("%d\n", packet_header->len);
    ethernet_protocol = (struct ether_header*)packet_content;
    printf("Ethernet type is:\n");
    ethernet_type = ntohs(ethernet_protocol->ether_type);
    printf("%04x\n", ethernet_type);
    switch (ethernet_type) {
        case 0x0800:
            printf("The network layer is IP protocol\n");
            break;
        case 0x0806:
            printf("The network layer is ARP protocol\n");
            break;
        case 0x8035:
            printf("The network layer is RARP protocol\n");
            break;
        case 0x86dd:
            printf("The network layer is IPv6 protocol\n");
            break;
        default:
            break;
    }
    printf("Mac Source Address is : \n");
    mac_string = ethernet_protocol->ether_shost;
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *mac_string, *(mac_string+1), *(mac_string+2), *(mac_string+3), *(mac_string+4), *(mac_string+5));
    printf("Mac Destination Address is : \n");
    mac_string = ethernet_protocol->ether_dhost;
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *mac_string, *(mac_string+1), *(mac_string+2), *(mac_string+3), *(mac_string+4), *(mac_string+5));
    printf("***********************************************\n");
    switch (ethernet_type) {
        case 0x0806:
            arp_protocol_packet_callback(argument, packet_header, packet_content);
            break;
        case 0x0800:
            ip_protocol_packet_callback(argument, packet_header, packet_content);
            break;
//        case 0x86dd:
//            ipv6_protocol_packet_callback(argument, packet_header, packet_content);
//            break;
    }
}

bool check(char *temp){
    for (int i = 0; i < strlen(temp); ++i) {
        if (temp[i] < '0' || temp[i] > '9'){
            if (temp[i] != '-'){
                return false;
            } else if (i != 0){
                return false;
            }
        }
    }
    return true;
}

void menu(){
    cout << "*****************************" <<endl;
    cout << "0.Quit" <<endl;
    cout << "1.Capture any packet" <<endl;
    cout << "2.Capture ethernet packet" <<endl;
    cout << "3.Capture arp protocol packet" <<endl;
    cout << "4.See localhost detail" <<endl;
    cout << "5.Capture ip protocol packet" <<endl;
    cout << "6.Capture tcp protocol packet" <<endl;
    cout << "7.Capture udp protocol packet" <<endl;
    cout << "8.Capture icmp protocol packet" <<endl;
    cout << "*****************************" <<endl;
}

char catchPacket(pcap_t *pcap_handle){
    int num;
    char temp[100],choice;
    cout << "input the number of packets you wanna capture!(-1 means infinity)" << endl;
    cin >> temp;
    while (!check(temp)){
        cout << "wrong number!" << endl;
        cin >> temp;
    }
    num = atoi(temp);
    if (num == 0) {
        menu();
        cin >> choice;
        return choice;
    }
    if (num < -1){
        cout << "wrong number!" << endl;
        menu();
        cin >> choice;
        return choice;
    }
    pcap_loop(pcap_handle, num, ethernet_protocol_packet_callback, NULL);
    menu();
    cin >> choice;
    return choice;
}

int main() {
    int num;
    char temp[100],choice;
    pcap_t *pcap_handle;
    bpf_u_int32 net_mask;
    bpf_u_int32 net_ip;
    char *net_ip_string;
    char *net_mask_string;
    struct  in_addr net_ip_address;
    struct in_addr net_mask_address;
    string bpf_filter_string;
    pcap_lookupnet(net_interface, &net_ip, &net_mask, error_content);
    pcap_handle = pcap_open_live(net_interface, BUFSIZ, 1, 0, error_content);
    struct bpf_program bpf_filter;
    menu();
    cin >> choice;
    while (choice != '0'){
        switch (choice) {
            case '1':
                cout << "input the number of packets you wanna capture!(-1 means infinity)" << endl;
                cin >> temp;
                while (!check(temp)){
                    cout << "wrong number!" << endl;
                    cin >> temp;
                }
                num = atoi(temp);
                if (num == 0) {
                    menu();
                    cin >> choice;
                    break;
                }
                if (num < -1){
                    cout << "wrong number!" << endl;
                    menu();
                    cin >> choice;
                    break;
                }
                pcap_loop(pcap_handle, num, packet_callback, NULL);
                pcap_close(pcap_handle);
                pcap_handle = pcap_open_live(net_interface, BUFSIZ, 1, 0, error_content);
                menu();
                cin >> choice;
                break;
            case '2':
                choice = catchPacket(pcap_handle);
                pcap_close(pcap_handle);
                pcap_handle = pcap_open_live(net_interface, BUFSIZ, 1, 0, error_content);
                break;
            case '3':
                bpf_filter_string = "arp";
                pcap_compile(pcap_handle, &bpf_filter, bpf_filter_string.c_str(), 0, net_ip);
                pcap_setfilter(pcap_handle, &bpf_filter);
                choice = catchPacket(pcap_handle);
                pcap_close(pcap_handle);
                pcap_handle = pcap_open_live(net_interface, BUFSIZ, 1, 0, error_content);
            case '4':
                printf("Network Interface is : %s\n", net_interface);
                net_ip_address.s_addr = net_ip;
                net_ip_string = inet_ntoa(net_ip_address);
                printf("Network Address is : %s\n", net_ip_string);
                net_mask_address.s_addr = net_mask;
                net_mask_string = inet_ntoa(net_mask_address);
                printf("Network Mask Address is: %s \n", net_mask_string);
                menu();
                cin >> choice;
                break;
            case '5':
                bpf_filter_string = "ip";
                pcap_compile(pcap_handle, &bpf_filter, bpf_filter_string.c_str(), 0, net_ip);
                pcap_setfilter(pcap_handle, &bpf_filter);
                choice = catchPacket(pcap_handle);
                pcap_close(pcap_handle);
                pcap_handle = pcap_open_live(net_interface, BUFSIZ, 1, 0, error_content);
                break;
            case '6':
                bpf_filter_string = "tcp";
                pcap_compile(pcap_handle, &bpf_filter, bpf_filter_string.c_str(), 0, net_ip);
                pcap_setfilter(pcap_handle, &bpf_filter);
                choice = catchPacket(pcap_handle);
                pcap_close(pcap_handle);
                pcap_handle = pcap_open_live(net_interface, BUFSIZ, 1, 0, error_content);
                break;
            case '7':
                bpf_filter_string = "udp";
                pcap_compile(pcap_handle, &bpf_filter, bpf_filter_string.c_str(), 0, net_ip);
                pcap_setfilter(pcap_handle, &bpf_filter);
                choice = catchPacket(pcap_handle);
                pcap_close(pcap_handle);
                pcap_handle = pcap_open_live(net_interface, BUFSIZ, 1, 0, error_content);
                break;
            case '8':
                bpf_filter_string = "icmp";
                pcap_compile(pcap_handle, &bpf_filter, bpf_filter_string.c_str(), 0, net_ip);
                pcap_setfilter(pcap_handle, &bpf_filter);
                choice = catchPacket(pcap_handle);
                pcap_close(pcap_handle);
                pcap_handle = pcap_open_live(net_interface, BUFSIZ, 1, 0, error_content);
                break;
            default:
                cout << "wrong choice!" << endl;
                menu();
                cin >> choice;
                break;
        }
    }
    pcap_close(pcap_handle);
    return 0;
}
