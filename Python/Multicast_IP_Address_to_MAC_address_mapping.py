def mcast_ip_to_mac(ip_string):
    l = list(ip_string.split('.'))
    return f"01:00:5E:{hex(int(l[1]) & 0x7f)[2:]:0>2}:{hex(int(l[2]))[2:]:0>2}:\
{hex(int(l[3]))[2:]:0>2}".upper() 


if __name__ == "__main__":
    print(mcast_ip_to_mac("224.128.0.1"))
    print(mcast_ip_to_mac("224.0.0.6"))
    print(mcast_ip_to_mac("224.0.0.251"))
    
        
