#ifndef __MQTT_REQUESTS_H__
#define __MQTT_REQUESTS_H__

#include <mqtt.h>


/**
 * @file 
 */

 /**
 * @brief Serialize an mqtt_fixed_header and write it to \p buf.
 * @ingroup packers
 * 
 * @note This function performs complete error checking and a positive return value
 *       guarantees the entire packet will fit into the given buffer.
 * 
 * @param[out] buf the buffer to write to.
 * @param[in] bufsz the maximum number of bytes that can be put in to \p buf.
 * @param[in] fixed_header the fixed header that will be serialized.
 * 
 * @returns The number of bytes written to \p buf, or 0 if \p buf is too small, or a 
 *          negative value if there was a protocol violation.
 */
ssize_t mqtt_pack_fixed_header(uint8_t *buf, size_t bufsz, const struct mqtt_fixed_header *fixed_header);

/**
 * @brief An enumeration of CONNECT packet flags.
 * @ingroup packers
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718030">
 * MQTT v3.1.1: CONNECT Variable Header.
 * </a> 
 */
enum MQTTConnectFlags {
    MQTT_CONNECT_RESERVED = 1u,
    MQTT_CONNECT_CLEAN_SESSION = 2u,
    MQTT_CONNECT_WILL_FLAG = 4u,
    MQTT_CONNECT_WILL_QOS_0 = (0u & 0x03) << 3,
    MQTT_CONNECT_WILL_QOS_1 = (1u & 0x03) << 3,
    MQTT_CONNECT_WILL_QOS_2 = (2u & 0x03) << 3,
    MQTT_CONNECT_WILL_RETAIN = 32u,
    MQTT_CONNECT_PASSWORD = 64u,
    MQTT_CONNECT_USER_NAME = 128u,
};

/**
 * @brief Serialize a connection request into a buffer. 
 * @ingroup packers
 * 
 * @param[out] buf the buffer to pack the connection request packet into.
 * @param[in] bufsz the number of bytes left in \p buf.
 * @param[in] client_id the ID that identifies the local client. \p client_id is a required 
 *                      parameter.
 * @param[in] will_topic the topic under which the local client's will message will be published.
 *                       Set to \c NULL for no will message. If \p will_topic is not \c NULL a
 *                       \p will_message must also be provided.
 * @param[in] will_message the will message to be published upon a unsuccessful disconnection of
 *                         the local client. Set to \c NULL if \p will_topic is \c NULL. 
 *                         \p will_message must \em not be \c NULL if \p will_topic is not 
 *                         \c NULL.
 * @param[in] user_name the username to be used to connect to the broker with. Set to \c NULL if 
 *                      no username is required.
 * @param[in] password the password to be used to connect to the broker with. Set to \c NULL if
 *                     no password is required.
 * @param[in] connect_flags additional MQTTConnectFlags to be set. The only flags that need to be
 *                          set manually are \c MQTT_CONNECT_CLEAN_SESSION, 
 *                          \c MQTT_CONNECT_WILL_QOS_X (for \c X &isin; {0, 1, 2}), and 
 *                          \c MQTT_CONNECT_WILL_RETAIN. Set to 0 if no additional flags are 
 *                          required.
 * @param[in] keep_alive the keep alive time in seconds. It is the responsibility of the clinet 
 *                       to ensure packets are sent to the server \em {at least} this frequently.
 * 
 * @note If there is a \p will_topic and no additional \p connect_flags are given, then by 
 *       default \p will_message will be published at QoS level 0.
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718028">
 * MQTT v3.1.1: CONNECT - Client Requests a Connection to a Server.
 * </a>
 * 
 * @returns The number of bytes put into \p buf, 0 if \p buf is too small to fit the CONNECT 
 *          packet, a negative value if there was a protocol violation.
 */
ssize_t mqtt_pack_connection_request(uint8_t* buf, size_t bufsz, 
                                     const char* client_id,
                                     const char* will_topic,
                                     const char* will_message,
                                     const char* user_name,
                                     const char* password,
                                     uint8_t connect_flags,
                                     uint16_t keep_alive);

/**
 * @brief An enumeration of the PUBLISH flags.
 * @ingroup packers
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718037">
 * MQTT v3.1.1: PUBLISH - Publish Message.
 * </a>
 */
enum MQTTPublishFlags {
    MQTT_PUBLISH_DUP = 8u,
    MQTT_PUBLISH_QOS_0 = ((0u << 1) & 0x06),
    MQTT_PUBLISH_QOS_1 = ((1u << 1) & 0x06),
    MQTT_PUBLISH_QOS_2 = ((2u << 1) & 0x06),
    MQTT_PUBLISH_RETAIN = 0x01
};

/**
 * @brief Serialize a PUBLISH request and put it in \p buf.
 * @ingroup packers
 * 
 * @param[out] buf the buffer to put the PUBLISH packet in.
 * @param[in] bufsz the maximum number of bytes that can be put into \p buf.
 * @param[in] topic_name the topic to publish \p application_message under.
 * @param[in] packet_id this packets packet ID.
 * @param[in] application_message the application message to be published.
 * @param[in] application_message_size the size of \p application_message in bytes.
 * @param[in] publish_flags The flags to publish \p application_message with. These include
 *                          the \c MQTT_PUBLISH_DUP flag, \c MQTT_PUBLISH_QOS_X (\c X &isin; 
 *                          {0, 1, 2}), and \c MQTT_PUBLISH_RETAIN flag.
 * 
 * @note The default QoS is level 0.
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718037">
 * MQTT v3.1.1: PUBLISH - Publish Message.
 * </a>
 * 
 * @returns The number of bytes put into \p buf, 0 if \p buf is too small to fit the PUBLISH 
 *          packet, a negative value if there was a protocol violation.
 */
ssize_t mqtt_pack_publish_request(uint8_t *buf, size_t bufsz,
                                  const char* topic_name,
                                  uint16_t packet_id,
                                  void* application_message,
                                  size_t application_message_size,
                                  uint8_t publish_flags);

/**
 * @brief Serialize a PUBACK, PUBREC, PUBREL, or PUBCOMP packet and put it in \p buf.
 * @ingroup packers
 * 
 * @param[out] buf the buffer to put the PUBXXX packet in.
 * @param[in] bufsz the maximum number of bytes that can be put into \p buf.
 * @param[in] control_type the type of packet. Must be one of: \c MQTT_CONTROL_PUBACK, 
 *                         \c MQTT_CONTROL_PUBREC, \c MQTT_CONTROL_PUBREL, 
 *                         or \c MQTT_CONTROL_PUBCOMP.
 * @param[in] packet_id the packet ID of the packet being acknowledged.
 * 
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718043">
 * MQTT v3.1.1: PUBACK - Publish Acknowledgement.
 * </a>
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718048">
 * MQTT v3.1.1: PUBREC - Publish Received.
 * </a>
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718053">
 * MQTT v3.1.1: PUBREL - Publish Released.
 * </a>
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718058">
 * MQTT v3.1.1: PUBCOMP - Publish Complete.
 * </a>
 * 
 * @returns The number of bytes put into \p buf, 0 if \p buf is too small to fit the PUBXXX 
 *          packet, a negative value if there was a protocol violation.
 */
ssize_t mqtt_pack_pubxxx_request(uint8_t *buf, size_t bufsz, 
                                 enum MQTTControlPacketType control_type,
                                 uint16_t packet_id);

/** 
 * @brief The maximum number topics that can be subscribed to in a single call to 
 *         mqtt_pack_subscribe_request.
 * @ingroup packers
 * 
 * @see mqtt_pack_subscribe_request
 */
#define MQTT_SUBSCRIBE_REQUEST_MAX_NUM_TOPICS 8

/** 
 * @brief Serialize a SUBSCRIBE packet and put it in \p buf.
 * @ingroup packers
 * 
 * @param[out] buf the buffer to put the SUBSCRIBE packet in.
 * @param[in] bufsz the maximum number of bytes that can be put into \p buf.
 * @param[in] packet_id the packet ID to be used.
 * @param[in] ... \c NULL terminated list of (\c {const char *topic_name}, \c {int max_qos_level})
 *                pairs.
 * 
 * @note The variadic arguments, \p ..., \em must be followed by a \c NULL. For example:
 * @code
 * ssize_t n = mqtt_pack_subscribe_request(buf, bufsz, 1234, "topic_1", 0, "topic_2", 2, NULL);
 * @endcode
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718063">
 * MQTT v3.1.1: SUBSCRIBE - Subscribe to Topics.
 * </a>
 * 
 * @returns The number of bytes put into \p buf, 0 if \p buf is too small to fit the SUBSCRIBE 
 *          packet, a negative value if there was a protocol violation.
 */
ssize_t mqtt_pack_subscribe_request(uint8_t *buf, size_t bufsz, 
                                    uint16_t packet_id, 
                                    ...); /* null terminated */

/** 
 * @brief The maximum number topics that can be subscribed to in a single call to 
 *         mqtt_pack_unsubscribe_request.
 * @ingroup packers
 * 
 * @see mqtt_pack_unsubscribe_request
 */
#define MQTT_UNSUBSCRIBE_REQUEST_MAX_NUM_TOPICS 8

/** 
 * @brief Serialize a UNSUBSCRIBE packet and put it in \p buf.
 * @ingroup packers
 * 
 * @param[out] buf the buffer to put the UNSUBSCRIBE packet in.
 * @param[in] bufsz the maximum number of bytes that can be put into \p buf.
 * @param[in] packet_id the packet ID to be used.
 * @param[in] ... \c NULL terminated list of \c {const char *topic_name}'s to unsubscribe from.
 * 
 * @note The variadic arguments, \p ..., \em must be followed by a \c NULL. For example:
 * @code
 * ssize_t n = mqtt_pack_unsubscribe_request(buf, bufsz, 4321, "topic_1", "topic_2", NULL);
 * @endcode
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718072">
 * MQTT v3.1.1: UNSUBSCRIBE - Unsubscribe from Topics.
 * </a>
 * 
 * @returns The number of bytes put into \p buf, 0 if \p buf is too small to fit the UNSUBSCRIBE 
 *          packet, a negative value if there was a protocol violation.
 */
ssize_t mqtt_pack_unsubscribe_request(uint8_t *buf, size_t bufsz, 
                                      uint16_t packet_id, 
                                      ...); /* null terminated */

/**
 * @brief Serialize a PINGREQ and put it into \p buf.
 * @ingroup packers
 * 
 * @param[out] buf the buffer to put the PINGREQ packet in.
 * @param[in] bufsz the maximum number of bytes that can be put into \p buf.
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718081">
 * MQTT v3.1.1: PINGREQ - Ping Request.
 * </a>
 * 
 * @returns The number of bytes put into \p buf, 0 if \p buf is too small to fit the PINGREQ
 *          packet, a negative value if there was a protocol violation.
 */
ssize_t mqtt_pack_ping_request(uint8_t *buf, size_t bufsz);

/**
 * @brief Serialize a DISCONNECT and put it into \p buf.
 * @ingroup packers
 * 
 * @param[out] buf the buffer to put the DISCONNECT packet in.
 * @param[in] bufsz the maximum number of bytes that can be put into \p buf.
 * 
 * @see <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718090">
 * MQTT v3.1.1: DISCONNECT - Disconnect Notification.
 * </a>
 * 
 * @returns The number of bytes put into \p buf, 0 if \p buf is too small to fit the DISCONNECT 
 *          packet, a negative value if there was a protocol violation.
 */
ssize_t mqtt_pack_disconnect(uint8_t *buf, size_t bufsz);

#endif