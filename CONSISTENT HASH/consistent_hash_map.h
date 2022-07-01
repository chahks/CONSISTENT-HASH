#include <map>
#include <string>
#include <list>
#include <functional> 
#include <algorithm>

#ifndef __CONSISTENT_HASH_H__
#define __CONSISTENT_HASH_H__

//consistent hash�Ľڵ����͡�
//һԪ�������󡣽���T���Ͷ�����Ϊ����������һ��������Ϊ��hashֵ����hashֵ���������ڲ�������Hash�������ڲ�����result_type ָ���������ε����͡�
//template<class T>��template<typename T> �������������庯��ģ�����ģ�壬��ʹ����û�б��ʵ�����
//����������ģ�庯��ʱ������T�ͻᱻ����ʱ�����������棬�����е�ֵ�Ͳ������������͡�
template <typename T,typename Hash,typename Alloc = std::allocator<std::pair<const typename Hash::result_type, T > > >

class consistent_hash_map
{
public:
    //typedef�����˴������͵ı�������typename���߱�����map_type::iterator��һ�����Ͷ�����һ����Ա��
    typedef typename Hash::result_type size_type;
    //ʹ��std::map������ڵ�
    //map������ģ���࣬��Ҫ�ؼ��ֺʹ洢��������ģ����������磺
    //std: map<int, CString> enumMap;�����Ͷ�����һ����int��Ϊ�ؼ��ּ���CString��Ŀ��map����
    //Alloc==less<type>()  ��С�������� grater<type>()  �Ӵ�С���� 
    typedef std::map<size_type, T, std::less<size_type>, Alloc> map_type;
    typedef typename map_type::value_type value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    //map<int, int>::iterator it; ����һ������ָ��it�� it->first Ϊ������ֵ��it->second Ϊֵ��
    typedef typename map_type::iterator iterator;//���������������ָ��
    typedef typename map_type::reverse_iterator reverse_iterator;//���������
    typedef Alloc allocator_type;

    consistent_hash_map() {}
    ~consistent_hash_map() {}

    std::size_t size() const {
        return nodes_.size();
    }

    bool empty() const {//�ж�consistent_hash_map�Ƿ�Ϊ��
        return nodes_.empty();
    }
    //����һ���ڵ㣬�������ֵ��bool����Ϊ�棬iterator��Ϊָ�����ڵ�ĵ����������boolΪ�٣���ʾ����ʧ�ܣ�iteratorָ���Ѿ����ڵĽڵ㡣
    //����ʧ����Ϊ�ڵ��Ѿ����ڻ����ǽڵ��hashֵ�������ڵ㷢����ͻ��
    //pair�ǽ�2��������ϳ�һ�����ݣ�����Ҫ����������ʱ�Ϳ���ʹ��pair����stl�е�map���ǽ�key��value����һ��������
    std::pair<iterator, bool> insert(const T& node) {
        size_type hash = hasher_(node);
        return nodes_.insert(value_type(hash, node));
    }

    iterator find(size_type hash) {//ͨ�������hashֵ�Ҷ�����consistent_hash�ж�Ӧ�Ľڵ�ĵ�����
        if (nodes_.empty()) {
            return nodes_.end();
        }

        iterator it = nodes_.lower_bound(hash);

        if (it == nodes_.end()) {
            it = nodes_.begin();
        }

        return it;
    }

    iterator begin() { return nodes_.begin(); }//���ض�Ӧ������
    iterator end() { return nodes_.end(); }
    reverse_iterator rbegin() { return nodes_.rbegin(); }//���ض�Ӧ�ķ��������
    reverse_iterator rend() { return nodes_.rend(); }


private:

    Hash hasher_;
    map_type nodes_;
};


#endif