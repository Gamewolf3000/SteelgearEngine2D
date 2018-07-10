#include "../../SteelgearEngine2D/Headers/SGTransformManager2D.h"
#include <math.h>

SG::SGTransformManager2D::SGTransformManager2D(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd) : SGManager(startNr, expandAmount, eventsBackEnd, ComponentType::COMPONENT_TRANSFORM)
{
	//eventHandler->BindFunctionToEvent<SGEntity2D*, SGEntity2DHandle&>(BackEndEvent::GET_TRANSFORM, [this](Transform2D* ptr, SGEntity2DHandle& toGet)
	//{
	//	SGEntity2D* temp = nullptr;
	//	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, temp, toGet);

	//	ptr = &objects[temp->components[int(componentType)]];
	//});
}

SG::SGTransformManager2D::~SGTransformManager2D()
{
}

void SG::SGTransformManager2D::CreateParentChildRelation(SGEntity2DHandle& ent)
{
	ParentChildRelation temp;

	int index = objects.Add(temp);

	this->AddComponentToEntity(ent, index);
}

void SG::SGTransformManager2D::Move(SGEntity2DHandle& ent, double xMovement, double yMovement)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	temp->xPos += xMovement;
	temp->yPos += yMovement;

	if (temp->components[int(componentType)] != -1)
	{
		for (auto& child : objects[temp->components[int(componentType)]].children)
		{
			Move(child, xMovement, yMovement);
		}
	}
}

void SG::SGTransformManager2D::SetPos(SGEntity2DHandle & ent, double xPos, double yPos)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	double xDif;
	double yDif;

	if (temp->components[int(componentType)] != -1 && objects[temp->components[int(componentType)]].children.size())
	{
		xDif = xPos - temp->xPos;
		yDif = yPos - temp->yPos;
		
		for (auto& child : objects[temp->components[int(componentType)]].children)
		{
			Move(child, xDif, yDif);
		}
	}

	temp->xPos = xPos;
	temp->yPos = yPos;


}

void SG::SGTransformManager2D::Rotate(SGEntity2DHandle & ent, double rotationAmount)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	float s;
	float c;

	if (temp->components[int(componentType)] != -1 && objects[temp->components[int(componentType)]].children.size())
	{
		s = sin(rotationAmount);
		c = cos(rotationAmount);
		
		for (auto& child : objects[temp->components[int(componentType)]].children)
		{
			SGEntity2D* temp2 = nullptr;
			eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp2, &child);

			// translate point back to origin:
			double adjustedX = temp2->xPos - temp->xPos;
			double adjustedY = temp2->yPos - temp->yPos;

			// rotate point
			double xnew = adjustedX * c - adjustedY * s;
			double ynew = adjustedX * s + adjustedY * c;

			// translate point back:
			SetPos(child, xnew + temp->xPos, ynew + temp->yPos);

			Rotate(child, rotationAmount);
		}
	}

	temp->rotation += rotationAmount;

	
}

void SG::SGTransformManager2D::SetRotation(SGEntity2DHandle & ent, double rotation)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	float s;
	float c;

	if (temp->components[int(componentType)] != -1 && objects[temp->components[int(componentType)]].children.size())
	{
		s = sin(rotation - temp->rotation);
		c = cos(rotation - temp->rotation);

		for (auto& child : objects[temp->components[int(componentType)]].children)
		{
			SGEntity2D* temp2 = nullptr;
			eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp2, &child);

			// translate point back to origin:
			double adjustedX = temp2->xPos - temp->xPos;
			double adjustedY = temp2->yPos - temp->yPos;

			// rotate point
			double xnew = adjustedX * c - adjustedY * s;
			double ynew = adjustedX * s + adjustedY * c;

			// translate point back:
			SetPos(child, xnew + temp->xPos, ynew + temp->yPos);

			SetRotation(child, rotation);
		}
	}

	temp->rotation = rotation;


}

void SG::SGTransformManager2D::BindChild(SGEntity2DHandle & parent, SGEntity2DHandle & child)
{
	SGEntity2D* parentEnt = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &parentEnt, &parent);

	SGEntity2D* childEnt = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &childEnt, &child);

	objects[parentEnt->components[int(componentType)]].children.push_back(child);
	objects[childEnt->components[int(componentType)]].parents.push_back(parent);
}

void SG::SGTransformManager2D::RemoveChild(SGEntity2DHandle & parent, SGEntity2DHandle & child)
{
	SGEntity2D* parentEnt = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &parentEnt, &parent);

	SGEntity2D* childEnt = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &childEnt, &child);

	for (auto& childToCheck : objects[parentEnt->components[int(componentType)]].children)
	{
		if (childToCheck == child)
		{
			childToCheck = objects[parentEnt->components[int(componentType)]].children.back();
			objects[parentEnt->components[int(componentType)]].children.pop_back();
			break;
		}
	}

	for (auto& parentToCheck : objects[childEnt->components[int(componentType)]].parents)
	{
		if (parentToCheck == parent)
		{
			parentToCheck = objects[childEnt->components[int(componentType)]].parents.back();
			objects[childEnt->components[int(componentType)]].parents.pop_back();
			break;
		}
	}
}

void SG::SGTransformManager2D::RemoveAllChildren(SGEntity2DHandle & ent)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	for (auto& child : objects[temp->components[int(componentType)]].children)
	{
		SGEntity2D* temp2 = nullptr;
		eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp2, &child);

		for (auto& parent : objects[temp2->components[int(componentType)]].parents)
		{
			if (parent == ent)
			{
				parent = objects[temp2->components[int(componentType)]].parents.back();
				objects[temp2->components[int(componentType)]].parents.pop_back();
				break;
			}
		}
	}

	objects[temp->components[int(componentType)]].children.clear();
}

void SG::SGTransformManager2D::RemoveAllParents(SGEntity2DHandle & ent)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	for (auto& parent : objects[temp->components[int(componentType)]].children)
	{
		SGEntity2D* temp2 = nullptr;
		eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp2, &parent);

		for (auto& child : objects[temp2->components[int(componentType)]].children)
		{
			if (child == ent)
			{
				child = objects[temp2->components[int(componentType)]].children.back();
				objects[temp2->components[int(componentType)]].children.pop_back();
				break;
			}
		}
	}

	objects[temp->components[int(componentType)]].parents.clear();
}

void SG::SGTransformManager2D::Remove(SGEntity2DHandle* ent)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, ent);

	if (temp->components[int(componentType)] != -1)
	{
		for (auto& child : objects[temp->components[int(componentType)]].children)
		{
			SGEntity2D* temp2 = nullptr;
			eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp2, &child);

			for (auto& parent : objects[temp2->components[int(componentType)]].parents)
			{
				if (parent == *ent)
				{
					parent = objects[temp2->components[int(componentType)]].parents.back();
					objects[temp2->components[int(componentType)]].parents.pop_back();
					break;
				}
			}
		}

		for (auto& parent : objects[temp->components[int(componentType)]].children)
		{
			SGEntity2D* temp2 = nullptr;
			eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp2, &parent);

			for (auto& child : objects[temp2->components[int(componentType)]].children)
			{
				if (child == *ent)
				{
					child = objects[temp2->components[int(componentType)]].children.back();
					objects[temp2->components[int(componentType)]].children.pop_back();
					break;
				}
			}
		}

		objects.Remove(temp->components[int(componentType)]);
		RemoveComponentFromEntity(temp);
	}

}


