#pragma once
#include <base_module.hpp>

namespace r2d2::communication{

	class communication_c : public base_module_c{
        
		public:

			communication_c(base_comm_c &comm) : base_module_c(comm){
				comm.listen_for_frames(frame_type::EXTERNAL);
			}

            receive_data_to_send();

	};

}