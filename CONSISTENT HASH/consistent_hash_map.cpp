

#include <stdint.h>
#include <iostream>
#include <string>
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include <boost/crc.hpp>

#include "consistent_hash_map.h"


//�����������б�
const char* nodes[] = {
    "192.168.1.100",
    "192.168.1.101",
    "192.168.1.102",
    "192.168.1.103",
    "192.168.1.104"
};
//����ڵ�
struct vnode_t {
    vnode_t() {}
    vnode_t(std::size_t n, std::size_t v) :node_id(n), vnode_id(v) {}

    std::string to_str() const {
        return boost::str(boost::format("%1%-%2%") % nodes[node_id] % vnode_id);
    }

    std::size_t node_id;//����ID,�����������б��е�����
    std::size_t vnode_id;//����ڵ�ID

};

//hasher,ʹ��CRC32��Ϊhash�㷨��ע����Ҫ����result_type
struct crc32_hasher {
    uint32_t operator()(const vnode_t& node) {
        boost::crc_32_type ret;
        std::string vnode = node.to_str();
        ret.process_bytes(vnode.c_str(), vnode.size());
        return ret.checksum();
    }
    typedef uint32_t result_type;
};

int main() {
    //Ϊÿ����������100������ڵ㣬Ȼ�����consistent_hash_map
    //map������ģ���࣬��Ҫ�ؼ��ֺʹ洢��������ģ����������磺
    //std: map<int, CString> enumMap;�����Ͷ�����һ����int��Ϊ�ؼ��ּ���CString��Ŀ��map����
    typedef consistent_hash_map<vnode_t, crc32_hasher> consistent_hash_t;
    consistent_hash_t consistent_hash_;//����ڵ�

    for (std::size_t i = 0; i < 5; ++i) {
        for (std::size_t j = 0; j < 100; j++) {
            consistent_hash_.insert(vnode_t(i, j));
        }
    }
    //����ĳ��hashֵ��Ӧ��vnode������
    consistent_hash_t::iterator it;//����ڵ�
    it = consistent_hash_.find(257894146);//ͨ�������hashֵ�Ҷ�����consistent_hash�ж�Ӧ�Ľڵ�ĵ�����
    //map<int, int>::iterator it; ����һ������ָ��it�� it->first Ϊ������ֵ��it->second Ϊֵ��
    //it -> first�Ǹýڵ��hashֵ��it -> second�Ǹ�����ڵ㡣
    std::cout << boost::format("node:%1%,vnode:%2%,hash:%3%") % nodes[it->second.node_id] % it->second.vnode_id % it->first << std::endl;

    //����consistent_hash�е����е�vnode,ͳ��ÿ������ڵ��key��������ÿ����������key������
    std::size_t sums[] = { 0,0,0,0,0 };//������������key������
    consistent_hash_t::iterator i = consistent_hash_.begin(); //��һ���ڵ�
    consistent_hash_t::reverse_iterator j = consistent_hash_.rbegin(); //���һ���ڵ�
    std::size_t n;
    //�������нڵ������key������,����������������key��������
    uint32_t priv = j->first;
    uint32_t cur;
    consistent_hash_t::iterator end = consistent_hash_.end();//���ص�����ĩβ
    while (i != end) {
        cur = i->first;
        n = cur - priv;
        std::cout << boost::format("vnode:%1%,hash:%2%,contains:%3%") % i->second.to_str() % cur % n << std::endl;
        sums[i->second.node_id] += n;//��������������key������
        priv = cur;
        i++;
    }

    for (std::size_t i = 0; i < 5; ++i) {
        std::cout << boost::format("node:%1% contains:%2%") % nodes[i] % sums[i] << std::endl;
    }
    system("pause");
    return 0;
}