
typedef
typename conditional< L==8u || L==4u || L==2u || L ==1u,     uint8_t,
                      typename conditional< L==16u,  uint16_t,
                                            typename conditional< L==32u,  uint32_t,
                                                                  typename conditional< L==64u,  uint64_t,
                                                                                        array < uint8_t, L/8 >
                                                                                        > :: type
                                                                  >::type
                                            >::type
                      >::type valueType;


typedef
typename conditional< L==8u || L==4u || L==2u || L ==1u,     uint8_t,
                      typename conditional< L<=16u,  uint16_t,
                                            typename conditional< L<=32u,  uint32_t, uint64_t  > :: type
                                            >::type
                      >::type valueIntType;




