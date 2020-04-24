//
// Generated file, do not edit! Created by nedtool 5.6 from IP_pck.msg.
//

#ifndef __IP_PCK_M_H
#define __IP_PCK_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>IP_pck.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet IP_pck
 * {
 *     int someField;
 *     string anotherField;
 *     double arrayField1[];
 *     double arrayField2[10];
 * }
 * </pre>
 */
class IP_pck : public ::omnetpp::cPacket
{
  protected:
    int someField;
    ::omnetpp::opp_string anotherField;
    double *arrayField1; // array ptr
    unsigned int arrayField1_arraysize;
    double arrayField2[10];

  private:
    void copy(const IP_pck& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const IP_pck&);

  public:
    IP_pck(const char *name=nullptr, short kind=0);
    IP_pck(const IP_pck& other);
    virtual ~IP_pck();
    IP_pck& operator=(const IP_pck& other);
    virtual IP_pck *dup() const override {return new IP_pck(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSomeField() const;
    virtual void setSomeField(int someField);
    virtual const char * getAnotherField() const;
    virtual void setAnotherField(const char * anotherField);
    virtual void setArrayField1ArraySize(unsigned int size);
    virtual unsigned int getArrayField1ArraySize() const;
    virtual double getArrayField1(unsigned int k) const;
    virtual void setArrayField1(unsigned int k, double arrayField1);
    virtual unsigned int getArrayField2ArraySize() const;
    virtual double getArrayField2(unsigned int k) const;
    virtual void setArrayField2(unsigned int k, double arrayField2);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const IP_pck& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, IP_pck& obj) {obj.parsimUnpack(b);}


#endif // ifndef __IP_PCK_M_H
