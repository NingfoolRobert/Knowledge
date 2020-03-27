#include "person.pb.h"
#include <iostream> 




int main()
{
	std::cout << PB::person::descriptor()->full_name() << std::endl;

	google::protobuf::Message* msg = NULL;
	const google::protobuf::Descriptor* des = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("PB.teacher");
	if(des)
	{
		const google::protobuf::Message* type = google::protobuf::MessageFactory::generated_factory()->GetPrototype(des);
		if(type)
		{
			msg = type->New();
			printf("create success\n");
		}

	}
	return 0;
}
