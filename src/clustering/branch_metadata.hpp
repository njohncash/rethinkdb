

template<class protocol_t>
class branch_metadata_t {

public:
    /* The initial state of the branch is composed of these regions of these
    versions. */

    std::vector<std::pair<typename protocol_t::region_t, version_t> > parents;

    /* These are the mailboxes to send reads and writes to if you want to
    operate on the branch. */

    typedef async_mailbox_t<void(
        typename protocol_t::read_t, order_token_t,
        typename async_mailbox_t<void(typename protocol_t::read_response_t)>::address_t
        )> read_mailbox_t;
    read_mailbox_t::address_t read_mailbox;

    typedef async_mailbox_t<void(
        typename protocol_t::write_t, order_token_t,
        typename async_mailbox_t<void(typename protocol_t::write_response_t)>::address_t
        )> write_mailbox_t;
    write_mailbox_t::address_t write_mailbox;

    /* These are the mirrors that the branch has. */

    typedef boost::uuids::uuid mirror_id_t;
    std::map<mirror_id_t, resource_metadata_t<backfiller_metadata_t<protocol_t> > > mirrors;

    /* This is how mirrors get in touch with the master when they start up */

    class mirror_data_t {

    public:
        typedef async_mailbox_t<void(
            typename protocol_t::write_t, repli_timestamp_t, order_token_t,
            async_mailbox_t<void()>::address_t
            )> write_mailbox_t;
        write_mailbox_t::address_t write_mailbox;

        typedef async_mailbox_t<void(
            typename protocol_t::write_t, order_token_t,
            typename async_mailbox_t<void(typename protocol_t::write_response_t)>::address_t
            )> writeread_mailbox_t;
        writeread_mailbox_t::address_t writeread_mailbox;

        typedef async_mailbox_t<void(
            typename protocol_t::read_t, order_token_t,
            typename async_mailbox_t<void(typename protocol_t::read_response_t)>::address_t
            )> read_mailbox_t;
        read_mailbox_t::address_t read_mailbox;

        mirror_data_t() { }
        mirror_data_t(const write_mailbox_t::address_t &wm) :
            write_mailbox(wm) { }
        mirror_data_t(const write_mailbox_t::address_t &wm, const writeread_mailbox_t::address_t &wrm, const read_mailbox_t::address_t &rm) :
            write_mailbox(wm), writeread_mailbox(wrm), read_mailbox(rm) { }
    };

    resource_metadata_t<registrar_metadata_t<mirror_data_t> > registrar;
};

