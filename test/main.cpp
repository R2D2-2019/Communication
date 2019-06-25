#include "ostream"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <mock_external.hpp>

TEST_CASE("mock_external_c, set_send_reaction compiles", ""){
    auto test_mock = r2d2::communication::mock_external_c();
    auto frame = r2d2::frame_external_s();
    std::function<void(
            const r2d2::frame_external_s&, 
            std::vector<r2d2::frame_external_s>&
            )> lam = [&](
                const r2d2::frame_external_s &fr, 
                std::vector<r2d2::frame_external_s> &retvec
                )->void{
                    retvec.push_back(r2d2::frame_external_s());
                    return;};
    test_mock.set_send_reaction(lam);

    test_mock.send(r2d2::frame_external_s());

    bool b = test_mock.receive(frame);

    REQUIRE(b == true);

}
