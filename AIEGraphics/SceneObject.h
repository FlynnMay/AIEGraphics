#pragma once
#include <list>
#include <glm/glm.hpp>
// THIS IS AN EXPERIMENTAL CLASS, IT ALSO HAS NO AFFILIATION WITH THE 'SCENE' CLASS
class SceneObject
{
public:
	SceneObject() {};
	~SceneObject() {};

	SceneObject* GetParent() { return m_parent; }
	glm::mat4* GetTransform() { return m_transform; }
	void SetTransform(glm::mat4* _transform) { m_transform = _transform; }
	void SetTransform(glm::mat4 _transform) { *m_transform = _transform; }
	glm::mat4 GetGlobalTransform() { return m_parent != nullptr ? *m_transform * m_parent->GetGlobalTransform() : *m_transform; }
	std::list<SceneObject*> GetChildren() { return m_children; }
	void ForceSetParent(SceneObject* _parent) { m_parent = _parent; }
	void SetParent(SceneObject* _parent) 
	{ 
		if (m_parent != nullptr)
			m_parent->RemoveChild(this);

		if (_parent != nullptr)
		{
			_parent->AddChild(this);
			ForceSetParent(_parent);
		}
	}

	void Update(float dt)
	{
		OnUpdate(dt);

		for (auto& child : m_children)
		{
			child->Update(dt);
		}
	}

	void Draw()
	{
		OnDraw();

		for (auto& child : m_children)
		{
			child->Draw();
		}
	}

	void RemoveChild(SceneObject* child)
	{
		if (child->GetParent() == this)
		{
			child->ForceSetParent(nullptr);
			m_children.remove(child);
		}
	}

	void AddChild(SceneObject* child)
	{
		if (child->GetParent() != nullptr)
			child->GetParent()->RemoveChild(child);

		child->ForceSetParent(this);
		m_children.push_back(child);
	}
	
	virtual void OnUpdate(float dt) = 0;
	virtual void OnDraw() = 0;

protected:
	std::list<SceneObject*> m_children;
	glm::mat4* m_transform;
	SceneObject* m_parent = nullptr;
};

